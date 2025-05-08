#pragma once

#include "src/renderer/vulkan/VulkanImage.h"

namespace Froth {
class VulkanImageView {
  friend class VulkanImage;

public:
  VulkanImageView() = default;
  VulkanImageView(VulkanImageView const &) = delete;
  VulkanImageView &operator=(VulkanImageView const &) = delete;
  VulkanImageView(VulkanImageView &&);
  VulkanImageView &operator=(VulkanImageView &&);
  ~VulkanImageView();

  VkFormat format() const { return m_Format; }
  operator VkImageView() const { return m_View; }
  void cleanup();

protected:
  VulkanImageView(const VulkanImage &image, VkFormat format, VkImageAspectFlags aspect);

private:
  VkFormat m_Format = VK_FORMAT_MAX_ENUM;
  VkImageView m_View = nullptr;
};

} // namespace Froth
