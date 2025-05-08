#pragma once
#include "VulkanFramebuffer.h"
#include "VulkanImageView.h"
#include "VulkanRenderPass.h"
#include "VulkanSurface.h"
#include "VulkanSwapchain.h"
#include <cstdint>

namespace Froth {

class VulkanSwapchainManager {
public:
  VulkanSwapchainManager() = default;
  VulkanSwapchainManager(const Window &window);
  VulkanSwapchainManager(const VulkanSwapchainManager &) = delete;
  VulkanSwapchainManager &operator=(const VulkanSwapchainManager &) = delete;

  void setShouldRebuild() { m_ShouldRebuild = true; }
  void onWindowFramebufferResize(uint32_t width, uint32_t height);
  void rebuild();

  const VulkanSurface &surface() const { return m_Surface; }
  const VulkanSwapchain &swapchain() const { return m_Swapchain; }
  const VulkanRenderPass &renderpass() const { return m_RenderPass; }
  const std::vector<VulkanFramebuffer> &framebuffers() const { return m_Framebuffers; }

private:
  VulkanSurface m_Surface;
  VulkanSwapchain m_Swapchain;
  VulkanImage m_DepthImage;
  VulkanImageView m_DepthImageView;
  VulkanRenderPass m_RenderPass;
  std::vector<VulkanFramebuffer> m_Framebuffers;

  bool m_ShouldRebuild = false;
  void createFramebuffers();
};

} // namespace Froth
