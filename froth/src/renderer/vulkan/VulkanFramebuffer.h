#pragma once

#include "VulkanRenderPass.h"
#include <vector>

namespace Froth {
class VulkanFramebuffer {
  friend class VulkanRenderer;

public:
  VulkanFramebuffer(const VulkanRenderPass &renderPass, const VkExtent2D &extent, const std::vector<VkImageView> &attachments);
  ~VulkanFramebuffer();
  VulkanFramebuffer(VulkanFramebuffer const &) = delete;
  void operator=(VulkanFramebuffer const &) = delete;
  VulkanFramebuffer(VulkanFramebuffer &&other) noexcept;

  operator VkFramebuffer() const { return m_Framebuffer; }

private:
  VkFramebuffer m_Framebuffer;

  void cleanup();
};

} // namespace Froth
