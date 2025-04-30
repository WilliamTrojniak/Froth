#pragma once

#include "src/renderer/vulkan/VulkanDevice.h"
#include "src/renderer/vulkan/VulkanImageView.h"
#include "src/renderer/vulkan/VulkanSwapchain.h"

namespace Froth {
class VulkanRenderPass {
  friend class VulkanRenderer;

public:
  VulkanRenderPass(const VulkanDevice &device, const VulkanSwapChain &swapchain, const VulkanImageView &depthImageView);
  ~VulkanRenderPass();
  VulkanRenderPass(VulkanRenderPass const &) = delete;
  void operator=(VulkanImageView const &) = delete;

  operator VkRenderPass() const { return m_RenderPass; }

private:
  const VulkanDevice &m_Device;
  VkRenderPass m_RenderPass;

  void cleanup();
};
} // namespace Froth
