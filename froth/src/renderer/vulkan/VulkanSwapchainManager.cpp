#include "VulkanSwapchainManager.h"
#include "src/platform/window/Window.h"
#include "src/renderer/vulkan/VulkanContext.h"
#include "vulkan/vulkan_core.h"

namespace Froth {

VulkanSwapchainManager::VulkanSwapchainManager(const Window &win)
    : m_Surface(win.createVulkanSurface()),
      m_Swapchain(VulkanSwapchain::create(m_Surface, nullptr)),
      m_DepthImage(VulkanImage::CreateInfo{
          .extent = m_Swapchain.extent(),
          .format = VK_FORMAT_D32_SFLOAT,
          .tiling = VK_IMAGE_TILING_OPTIMAL,
          .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
      }),
      m_DepthImageView(m_DepthImage.createView(VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT)),
      m_RenderPass(m_Swapchain.format().format, m_DepthImageView.format()) {
  createFramebuffers();
}

void VulkanSwapchainManager::onWindowFramebufferResize(uint32_t width, uint32_t height) {
  m_Surface.resize(width, height);
  rebuild();
}

void VulkanSwapchainManager::rebuild() {
  vkDeviceWaitIdle(VulkanContext::get().device());

  m_Framebuffers.clear();
  m_DepthImageView.cleanup();
  m_DepthImage.cleanup();

  VulkanSwapchain oldSwapchain = std::move(m_Swapchain);
  m_Swapchain = VulkanSwapchain::create(m_Surface, &oldSwapchain);
  m_DepthImage = VulkanImage(VulkanImage::CreateInfo{
      .extent = m_Swapchain.extent(),
      .format = VK_FORMAT_D32_SFLOAT,
      .tiling = VK_IMAGE_TILING_OPTIMAL,
      .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
  });

  m_DepthImageView = m_DepthImage.createView(VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT);

  // TODO: Rebuild render pass if dynamic viewport is not on

  m_ShouldRebuild = false;
  createFramebuffers();
}

void VulkanSwapchainManager::createFramebuffers() {
  m_Framebuffers.reserve(m_Swapchain.views().size());
  std::vector<VkImageView> framebufferAttachments(2);
  framebufferAttachments[1] = m_DepthImageView;
  for (size_t i = 0; i < m_Swapchain.views().size(); i++) {
    framebufferAttachments[0] = m_Swapchain.views()[i];
    m_Framebuffers.emplace_back(m_RenderPass, m_Swapchain.extent(), framebufferAttachments);
  }
}

} // namespace Froth
