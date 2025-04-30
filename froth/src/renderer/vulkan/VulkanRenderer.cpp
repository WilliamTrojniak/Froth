#include "VulkanRenderer.h"
#include "Defines.h"
#include "VulkanIndexBuffer.h"
#include "VulkanPipelineBuilder.h"
#include "VulkanPipelineLayout.h"
#include "VulkanShaderModule.h"
#include "VulkanSwapchain.h"
#include "VulkanVertex.h"
#include "VulkanVertexBuffer.h"
#include "src/core/events/ApplicationEvent.h"
#include "src/core/events/EventDispatcher.h"
#include "src/core/logger/Logger.h"
#include "src/platform/filesystem/Filesystem.h"
#include <cstdint>
#include <memory>
#include <vector>

namespace Froth {

#define BIND_FUNC(x) std::bind(&VulkanRenderer::x, this, std::placeholders::_1)

const uint32_t MAX_FRAMES_IN_FLIGHT = 2;

bool hasExtensions(const std::vector<const char *> &extensions) noexcept;
bool getRequiredExtensions(std::vector<const char *> &extensions) noexcept;
bool hasLayers(const std::vector<const char *> &layers) noexcept;

bool VulkanRenderer::s_Initialized = false;
VulkanInstance VulkanRenderer::s_Ctx{};

VulkanRenderer::VulkanRenderer(const Window &window)
    : m_Surface(window.createVulkanSurface(s_Ctx)),
      m_Device(s_Ctx, m_Surface),
      m_DescriptorSetLayout(m_Device),
      m_GraphicsCommandPool(m_Device, m_Device.getQueueFamilies().graphics.index) {

  std::vector<VkDescriptorSetLayout> descSetLayouts = {m_DescriptorSetLayout.data()};
  m_PipelineLayout = std::make_unique<VulkanPipelineLayout>(m_Device, descSetLayouts);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    m_CommandBuffers.emplace_back(m_Device, m_GraphicsCommandPool);
    m_ImageAvailableSemaphores.emplace_back(m_Device);
    m_RenderFinishedSemaphores.emplace_back(m_Device);
    m_FrameInFlightFences.emplace_back(m_Device, true);
  }

  recreateSwapchain();
}

VulkanRenderer::~VulkanRenderer() {}

std::unique_ptr<VulkanRenderer> VulkanRenderer::create(const Window &window) {
  if (!s_Initialized) {
    s_Ctx = VulkanInstance(nullptr); // TODO: Configurable allocator
    s_Initialized = true;
  }
  return std::unique_ptr<VulkanRenderer>(new VulkanRenderer(window));
}

void VulkanRenderer::shutdown() {
  vkDeviceWaitIdle(m_Device);
}

void VulkanRenderer::recreateSwapchain() {
  FROTH_DEBUG("Recreating swapchain");
  vkDeviceWaitIdle(m_Device);

  m_Pipeline = nullptr;
  m_Framebuffers.clear();
  m_RenderPass = nullptr;
  m_DepthImageView = nullptr;
  m_DepthImage = nullptr;
  m_Swapchain = std::make_unique<VulkanSwapChain>(m_Device, m_Surface, m_Swapchain.get());
  m_DepthImage = std::make_unique<VulkanImage>(m_Device, VulkanImage::CreateInfo{
                                                             .extent = {.width = m_Swapchain->extent().width,
                                                                        .height = m_Swapchain->extent().height},
                                                             .format = VK_FORMAT_D32_SFLOAT,
                                                             .tiling = VK_IMAGE_TILING_OPTIMAL,
                                                             .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                                         });
  m_DepthImageView = std::make_unique<VulkanImageView>(m_DepthImage->createView(VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT));
  m_RenderPass = std::make_unique<VulkanRenderPass>(m_Device, *m_Swapchain, *m_DepthImageView);

  m_Framebuffers.reserve(m_Swapchain->views().size());
  std::vector<VkImageView> framebufferAttachments(2);
  framebufferAttachments[1] = *m_DepthImageView;
  for (size_t i = 0; i < m_Swapchain->views().size(); i++) {
    framebufferAttachments[0] = m_Swapchain->views()[i];
    m_Framebuffers.emplace_back(m_Device, *m_RenderPass, m_Swapchain->extent(), framebufferAttachments);
  }

  std::vector<char> vertShaderCode = Filesystem::readFile("../playground/shaders/vert.spv");
  std::vector<char> fragShaderCode = Filesystem::readFile("../playground/shaders/frag.spv");

  VulkanShaderModule vertShaderModule = VulkanShaderModule(m_Device, vertShaderCode);
  VulkanShaderModule fragShaderModule = VulkanShaderModule(m_Device, fragShaderCode);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = m_Swapchain->extent().width;
  viewport.height = m_Swapchain->extent().height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = m_Swapchain->extent();

  m_Pipeline = VulkanPipelineBuilder()
                   .setVertexInput(Vertex::getInputDescription().getInfo())
                   .setShaders(vertShaderModule, fragShaderModule)
                   .setViewport(viewport, scissor)
                   .build(m_Device, *m_RenderPass, *m_PipelineLayout);
}

bool VulkanRenderer::onEvent(const Event &e) {
  EventDispatcher dispatcher = EventDispatcher(e);
  dispatcher.dispatch<WindowResizeEvent>(BIND_FUNC(onWindowResize));
  return dispatcher.isHandled();
}

bool VulkanRenderer::onWindowResize(WindowResizeEvent &e) {
  m_WindowResized = true;
  return false;
}

bool VulkanRenderer::beginFrame() {
  VkFence inFlightFence[] = {m_FrameInFlightFences[m_CurrentFrame]};
  vkWaitForFences(m_Device, 1, inFlightFence, VK_TRUE, UINT64_MAX);

  VkResult result = vkAcquireNextImageKHR(m_Device, *m_Swapchain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &m_CurrentImageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    m_WindowResized = false;
    recreateSwapchain();
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    FROTH_ERROR("Failed to acquire swapchain image");
  }

  vkResetFences(m_Device, 1, inFlightFence);
  vkResetCommandBuffer(m_CommandBuffers[m_CurrentFrame], 0);

  // Record command buffer
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0;
  beginInfo.pInheritanceInfo = nullptr;
  if (vkBeginCommandBuffer(m_CommandBuffers[m_CurrentFrame], &beginInfo) != VK_SUCCESS) {
    return false;
  }

  return true;
}

void VulkanRenderer::beginRenderPass() {
  // Render Pass Begin
  std::array<VkClearValue, 2> clearValues{};
  clearValues[0] = {0.0f, 0.0f, 0.0f, 1.0f};
  clearValues[1] = {1.0f, 0};

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = *m_RenderPass;
  renderPassInfo.framebuffer = m_Framebuffers[m_CurrentImageIndex];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = m_Swapchain->extent();
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();
  vkCmdBeginRenderPass(m_CommandBuffers[m_CurrentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

  // Bind pipeline
  vkCmdBindPipeline(m_CommandBuffers[m_CurrentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, *m_Pipeline);

  // TODO: Should this move elsewhere?
  // Viewport
  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = m_Swapchain->extent().width;
  viewport.height = m_Swapchain->extent().height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(m_CommandBuffers[m_CurrentFrame], 0, 1, &viewport);

  // Scissor
  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = m_Swapchain->extent();
  vkCmdSetScissor(m_CommandBuffers[m_CurrentFrame], 0, 1, &scissor);
}

void VulkanRenderer::endRenderPass() {
  vkCmdEndRenderPass(m_CommandBuffers[m_CurrentFrame]);
}

void VulkanRenderer::endFrame() {
  if (vkEndCommandBuffer(m_CommandBuffers[m_CurrentFrame]) != VK_SUCCESS) {
    FROTH_ERROR("Failed to record command buffer");
  }

  // Submit draw command buffer
  VkSubmitInfo submitInfo{};
  VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  VkSemaphore imageAvailableSemaphore[] = {m_ImageAvailableSemaphores[m_CurrentFrame]};
  VkSemaphore renderFinishedSemaphore[] = {m_RenderFinishedSemaphores[m_CurrentFrame]};
  VkCommandBuffer commandBuffer[] = {m_CommandBuffers[m_CurrentFrame]};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = imageAvailableSemaphore;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = commandBuffer;
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = renderFinishedSemaphore;
  if (vkQueueSubmit(m_Device.getQueueFamilies().graphics.queue, 1, &submitInfo, m_FrameInFlightFences[m_CurrentFrame]) != VK_SUCCESS) {
    FROTH_ERROR("Failed to submit draw command buffer");
  }

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = renderFinishedSemaphore;

  VkSwapchainKHR swapchains[] = {*m_Swapchain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapchains;
  presentInfo.pImageIndices = &m_CurrentImageIndex;
  presentInfo.pResults = nullptr;

  VkResult result = vkQueuePresentKHR(m_Device.getQueueFamilies().present.queue, &presentInfo);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_WindowResized) {
    m_WindowResized = false;
    recreateSwapchain();
  } else if (result != VK_SUCCESS) {
    FROTH_ERROR("Failed to present swap chain image");
  }

  m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

std::unique_ptr<VertexBuffer> VulkanRenderer::createVertexBuffer(size_t sizeBytes) {
  return std::make_unique<VulkanVertexBuffer>(m_Device, *this, sizeBytes, m_GraphicsCommandPool);
}

std::unique_ptr<IndexBuffer> VulkanRenderer::createIndexBuffer(size_t numIndices) {
  return std::make_unique<VulkanIndexBuffer>(m_Device, *this, numIndices * sizeof(uint32_t), m_GraphicsCommandPool);
}

void VulkanRenderer::bindVertexBuffer(const VulkanVertexBuffer &vertexBuffer) const {
  // TODO: Handle dynamic offsets
  VkDeviceSize offsets[] = {0};
  VkBuffer vertexBuffers[] = {vertexBuffer};
  vkCmdBindVertexBuffers(m_CommandBuffers[m_CurrentFrame], 0, 1, vertexBuffers, offsets);
}

void VulkanRenderer::bindIndexBuffer(const VulkanIndexBuffer &indexBuffer) const {
  // TODO: Handle offsets
  vkCmdBindIndexBuffer(m_CommandBuffers[m_CurrentFrame], indexBuffer, 0, VK_INDEX_TYPE_UINT32);

  // TODO: Seperate into another call?
  vkCmdDrawIndexed(m_CommandBuffers[m_CurrentFrame], indexBuffer.indexCount(), 1, 0, 0, 0);
}

} // namespace Froth
