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
  void operator=(VulkanImageView const &) = delete;

  operator VkRenderPass() const { return m_RenderPass; }

private:
  VkRenderPass m_RenderPass = nullptr;

  void cleanup();
};
} // namespace Froth
