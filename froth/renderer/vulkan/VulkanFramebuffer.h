#pragma once

#include "renderer/vulkan/VulkanDevice.h"
#include "renderer/vulkan/VulkanRenderPass.h"
#include <vector>

namespace Froth {
class VulkanFramebuffer {
  friend class VulkanRenderer;

public:
  ~VulkanFramebuffer();
  VulkanFramebuffer(VulkanFramebuffer const &) = delete;
  void operator=(VulkanFramebuffer const &) = delete;
  VulkanFramebuffer(VulkanFramebuffer &&other) noexcept;
  VulkanFramebuffer(const VulkanDevice &device, const VulkanRenderPass &renderPass, const VkExtent2D &extent, const std::vector<VkImageView> &attachments);

private:
  const VulkanDevice &m_Device;
  VkFramebuffer m_Framebuffer;

  void cleanup();
};

} // namespace Froth
