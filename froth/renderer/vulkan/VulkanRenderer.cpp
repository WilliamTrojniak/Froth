#include "VulkanRenderer.h"
#include "Defines.h"
#include "platform/filesystem/Filesystem.h"
#include "renderer/vulkan/VulkanShaderModule.h"
#include "vulkan/vulkan_core.h"
#include <memory>
#include <vector>

namespace Froth {

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
                                 .extent = {.width = m_Swapchain.extent().width, .height = m_Swapchain.extent().height},
                                 .format = VK_FORMAT_D32_SFLOAT,
                                 .tiling = VK_IMAGE_TILING_OPTIMAL,
                                 .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                             }),
      m_DepthImageView(m_DepthImage.createView(VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT)), m_RenderPass(m_Device, m_Swapchain, m_DepthImageView), m_DescriptorSetLayout(m_Device), m_PipelineLayout(m_Device, {m_DescriptorSetLayout.data()}) {

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
