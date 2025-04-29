#pragma once

#include "renderer/vulkan/VulkanDevice.h"
#include "renderer/vulkan/VulkanImageView.h"
#include "renderer/vulkan/VulkanSwapchain.h"

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
