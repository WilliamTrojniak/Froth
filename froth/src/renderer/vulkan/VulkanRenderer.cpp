#include "VulkanRenderer.h"
#include "Defines.h"
#include "VulkanIndexBuffer.h"
#include "VulkanPipelineBuilder.h"
#include "VulkanPipelineLayout.h"
#include "VulkanVertex.h"
#include "VulkanVertexBuffer.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "src/core/events/ApplicationEvent.h"
#include "src/core/events/EventDispatcher.h"
#include "src/renderer/vulkan/VulkanContext.h"
#include "src/resources/materials/Material.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>
#include <memory>
#include <vector>

namespace Froth {

#define BIND_FUNC(x) std::bind(&VulkanRenderer::x, this, std::placeholders::_1)

const uint32_t MAX_FRAMES_IN_FLIGHT = 2;

bool hasExtensions(const std::vector<const char *> &extensions) noexcept;
bool getRequiredExtensions(std::vector<const char *> &extensions) noexcept;
bool hasLayers(const std::vector<const char *> &layers) noexcept;

VulkanRenderer::VulkanRenderer(const Window &window, _tag)
    : m_SwapchainManager(window),
      m_DescriptorSetLayout(),
      m_GraphicsCommandPool(VulkanContext::get().device().getQueueFamilies().graphics.index) {

  std::vector<VkDescriptorSetLayout>
      descSetLayouts = {m_DescriptorSetLayout.data()};
  m_PipelineLayout = std::make_unique<VulkanPipelineLayout>(descSetLayouts);
}

VulkanRenderer::~VulkanRenderer() {
}

std::unique_ptr<VulkanRenderer> VulkanRenderer::create(const Window &window) {
  return std::make_unique<VulkanRenderer>(window, _tag{});
}

void VulkanRenderer::shutdown() {
  VulkanContext &vctx = VulkanContext::get();
  vkDeviceWaitIdle(vctx.device());
  vctx.cleanup();
}

bool VulkanRenderer::onEvent(const Event &e) {
  EventDispatcher dispatcher = EventDispatcher(e);
  dispatcher.dispatch<FramebufferResizeEvent>(BIND_FUNC(onFramebufferResize));
  return dispatcher.isHandled();
}

bool VulkanRenderer::onFramebufferResize(FramebufferResizeEvent &e) {
  m_SwapchainManager.onWindowFramebufferResize(e.width(), e.height());
  return false;
}

bool VulkanRenderer::beginFrame() {
  return m_SwapchainManager.beginFrame();
}

void VulkanRenderer::beginRenderPass() {
  // Render Pass Begin
  std::array<VkClearValue, 2> clearValues{};
  clearValues[0] = {0.0f, 0.0f, 0.0f, 1.0f};
  clearValues[1] = {1.0f, 0};

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = m_SwapchainManager.renderpass();
  renderPassInfo.framebuffer = m_SwapchainManager.currentFramebuffer();
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = m_SwapchainManager.swapchain().extent();
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();
  vkCmdBeginRenderPass(m_SwapchainManager.currentCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanRenderer::endRenderPass() {
  vkCmdEndRenderPass(m_SwapchainManager.currentCommandBuffer());
}

void VulkanRenderer::endFrame() {
  m_SwapchainManager.endFrame();
}

std::unique_ptr<VertexBuffer> VulkanRenderer::createVertexBuffer(size_t sizeBytes) {
  return std::make_unique<VulkanVertexBuffer>(*this, sizeBytes, m_GraphicsCommandPool);
}

std::unique_ptr<IndexBuffer> VulkanRenderer::createIndexBuffer(size_t numIndices) {
  return std::make_unique<VulkanIndexBuffer>(*this, numIndices * sizeof(uint32_t), m_GraphicsCommandPool);
}

void VulkanRenderer::pushConstants(const glm::mat4 &mat) const {
  vkCmdPushConstants(m_SwapchainManager.currentCommandBuffer(), *m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &mat);
}

void VulkanRenderer::bindMaterial(const Material &mat) {
  // FIXME: This assumes only one material
  if (!m_Pipeline) {
    m_Pipeline = buildPipeline(mat);
  }
  // Bind pipeline
  vkCmdBindPipeline(m_SwapchainManager.currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, *m_Pipeline);

  // TODO: Should this move elsewhere?
  // Viewport
  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = m_SwapchainManager.swapchain().extent().width;
  viewport.height = m_SwapchainManager.swapchain().extent().height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(m_SwapchainManager.currentCommandBuffer(), 0, 1, &viewport);

  // Scissor
  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = m_SwapchainManager.swapchain().extent();
  vkCmdSetScissor(m_SwapchainManager.currentCommandBuffer(), 0, 1, &scissor);
}

void VulkanRenderer::bindVertexBuffer(const VulkanVertexBuffer &vertexBuffer) const {
  // TODO: Handle dynamic offsets
  VkDeviceSize offsets[] = {0};
  VkBuffer vertexBuffers[] = {vertexBuffer};
  vkCmdBindVertexBuffers(m_SwapchainManager.currentCommandBuffer(), 0, 1, vertexBuffers, offsets);
}

void VulkanRenderer::bindIndexBuffer(const VulkanIndexBuffer &indexBuffer) const {
  // TODO: Handle offsets
  vkCmdBindIndexBuffer(m_SwapchainManager.currentCommandBuffer(), indexBuffer, 0, VK_INDEX_TYPE_UINT32);

  // TODO: Seperate into another call?
  vkCmdDrawIndexed(m_SwapchainManager.currentCommandBuffer(), indexBuffer.indexCount(), 1, 0, 0, 0);
}

std::unique_ptr<VulkanPipeline> VulkanRenderer::buildPipeline(const Material &mat) {
  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = m_SwapchainManager.swapchain().extent().width;
  viewport.height = m_SwapchainManager.swapchain().extent().height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = m_SwapchainManager.swapchain().extent();

  return VulkanPipelineBuilder()
      .setVertexInput(Vertex::getInputDescription().getInfo())
      .setShaders(mat.frag().getVulkanShaderModule(), mat.vert().getVulkanShaderModule())
      .setViewport(viewport, scissor)
      .build(m_SwapchainManager.renderpass(), *m_PipelineLayout);
}

} // namespace Froth
