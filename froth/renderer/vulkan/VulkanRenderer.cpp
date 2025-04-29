#include "VulkanRenderer.h"
#include "Defines.h"
#include "platform/filesystem/Filesystem.h"
#include "renderer/vulkan/VulkanPipelineBuilder.h"
#include "renderer/vulkan/VulkanShaderModule.h"
#include "renderer/vulkan/VulkanVertex.h"
#include "vulkan/vulkan_core.h"
#include <memory>
#include <vector>

namespace Froth {
const uint32_t MAX_FRAMES_IN_FLIGHT = 2;

bool hasExtensions(const std::vector<const char *> &extensions) noexcept;
bool getRequiredExtensions(std::vector<const char *> &extensions) noexcept;
bool hasLayers(const std::vector<const char *> &layers) noexcept;

bool VulkanRenderer::s_Initialized = false;
VulkanInstance VulkanRenderer::s_Ctx{};

VulkanRenderer::VulkanRenderer(const Window &window)
    : m_Surface(window.createVulkanSurface(s_Ctx)),
      m_Device(s_Ctx, m_Surface),
      m_Swapchain(m_Device, window, m_Surface),
      m_DepthImage(m_Device, VulkanImage::CreateInfo{
                                 .extent = {.width = m_Swapchain.extent().width,
                                            .height = m_Swapchain.extent().height},
                                 .format = VK_FORMAT_D32_SFLOAT,
                                 .tiling = VK_IMAGE_TILING_OPTIMAL,
                                 .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                             }),
      m_DepthImageView(m_DepthImage.createView(VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT)), m_RenderPass(m_Device, m_Swapchain, m_DepthImageView), m_DescriptorSetLayout(m_Device), m_PipelineLayout(m_Device, {m_DescriptorSetLayout.data()}), m_GraphicsCommandPool(m_Device, m_Device.getQueueFamilies().graphics.index) {

  m_Framebuffers.reserve(m_Swapchain.views().size());
  std::vector<VkImageView> framebufferAttachments(2);
  framebufferAttachments[1] = m_DepthImageView.view();
  for (size_t i = 0; i < m_Swapchain.views().size(); i++) {
    framebufferAttachments[0] = m_Swapchain.views()[i];
    m_Framebuffers.emplace_back(m_Device, m_RenderPass, m_Swapchain.extent(), framebufferAttachments);
  }

  std::vector<char> vertShaderCode = Filesystem::readFile("../playground/shaders/vert.spv");
  std::vector<char> fragShaderCode = Filesystem::readFile("../playground/shaders/frag.spv");

  VulkanShaderModule vertShaderModule = VulkanShaderModule(m_Device, vertShaderCode);
  VulkanShaderModule fragShaderModule = VulkanShaderModule(m_Device, fragShaderCode);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = m_Swapchain.extent().width;
  viewport.height = m_Swapchain.extent().height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = m_Swapchain.extent();

  m_Pipeline = VulkanPipelineBuilder()
                   .setVertexInput(Vertex::getInputDescription().getInfo())
                   .setShaders(vertShaderModule, fragShaderModule)
                   .setViewport(viewport, scissor)
                   .build(m_Device, m_RenderPass, m_PipelineLayout);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    m_CommandBuffers.emplace_back(m_Device, m_GraphicsCommandPool);
    m_ImageAvailableSemaphores.emplace_back(m_Device);
    m_RenderFinishedSemaphores.emplace_back(m_Device);
    m_FrameInFlightFences.emplace_back(m_Device, true);
  }

  m_VertexBuffer = std::make_unique<VulkanVertexBuffer>(m_Device, sizeof(Vertex) * 100, m_GraphicsCommandPool);
  std::vector<Vertex> vData = {
      {glm::vec3(1, 1, 1), glm::vec3(1.0, 0.0, 0), glm::vec2(1.0, 0.0)},
      {glm::vec3(1, 1, 1), glm::vec3(1.0, 0.0, 0), glm::vec2(1.0, 0.0)},
      {glm::vec3(1, 1, 1), glm::vec3(1.0, 0.0, 0), glm::vec2(1.0, 0.0)}};
  m_VertexBuffer->write(vData.data(), sizeof(Vertex) * vData.size());

  m_IndexBuffer = std::make_unique<VulkanIndexBuffer>(m_Device, sizeof(uint32_t) * 3, m_GraphicsCommandPool);
  std::vector<uint32_t> iData = {0, 1, 2};
  m_IndexBuffer->write(iData.data(), sizeof(uint32_t) * iData.size());
}

VulkanRenderer::~VulkanRenderer() {
  shutdown();
}

std::unique_ptr<VulkanRenderer> VulkanRenderer::create(const Window &window) {
  if (!s_Initialized) {
    s_Ctx = VulkanInstance(nullptr); // TODO: Configurable allocator
    s_Initialized = true;
  }
  return std::unique_ptr<VulkanRenderer>(new VulkanRenderer(window));
}

void VulkanRenderer::shutdown() noexcept {
  s_Initialized = false;
}

} // namespace Froth
