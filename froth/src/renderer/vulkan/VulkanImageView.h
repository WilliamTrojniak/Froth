#pragma once

#include "src/renderer/vulkan/VulkanDevice.h"
#include "src/renderer/vulkan/VulkanImage.h"

namespace Froth {
class VulkanImageView {
  friend class VulkanImage;

public:
  VulkanImageView(VulkanImageView const &) = delete;
  void operator=(VulkanImageView const &) = delete;
  ~VulkanImageView();

  VulkanImageView(VulkanImageView &&);

  VkFormat format() const { return m_Format; }
  operator VkImageView() const { return m_View; }

protected:
  VulkanImageView(const VulkanDevice &device, const VulkanImage &image, VkFormat format, VkImageAspectFlags aspect);

private:
  const VulkanDevice &m_Device;
  VkFormat m_Format;
  VkImageView m_View;

  void cleanup();
};

} // namespace Froth
