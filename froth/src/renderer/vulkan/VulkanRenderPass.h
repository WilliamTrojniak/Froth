#pragma once

#include "VulkanImageView.h"

namespace Froth {
class VulkanRenderPass {
  friend class VulkanRenderer;

public:
  VulkanRenderPass() = default;
  VulkanRenderPass(VkFormat swapchainFormat, VkFormat depthImageViewFormat);
  ~VulkanRenderPass();
  VulkanRenderPass(VulkanRenderPass const &) = delete;
  void operator=(VulkanRenderPass const &) = delete;
  VulkanRenderPass(VulkanRenderPass &&) noexcept;
  VulkanRenderPass &operator=(VulkanRenderPass &&) noexcept;

  operator VkRenderPass() const { return m_RenderPass; }

private:
  VkRenderPass m_RenderPass = nullptr;

  void cleanup();
};
} // namespace Froth
