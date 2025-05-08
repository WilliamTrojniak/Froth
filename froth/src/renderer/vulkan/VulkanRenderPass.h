#pragma once

#include "VulkanImageView.h"
#include "VulkanSwapchain.h"

namespace Froth {
class VulkanRenderPass {
  friend class VulkanRenderer;

public:
  VulkanRenderPass(const VulkanSwapChain &swapchain, const VulkanImageView &depthImageView);
  ~VulkanRenderPass();
  VulkanRenderPass(VulkanRenderPass const &) = delete;
  void operator=(VulkanImageView const &) = delete;

  operator VkRenderPass() const { return m_RenderPass; }

private:
  VkRenderPass m_RenderPass;

  void cleanup();
};
} // namespace Froth
