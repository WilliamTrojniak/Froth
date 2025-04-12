#pragma once

#include "renderer/vulkan/VulkanDevice.h"
#include "renderer/vulkan/VulkanImage.h"

namespace Froth {
class VulkanImageView {
  friend class VulkanImage;

public:
  VulkanImageView(VulkanImageView const &) = delete;
  void operator=(VulkanImageView const &) = delete;
  ~VulkanImageView();

protected:
  VulkanImageView(const VulkanDevice &device, const VulkanImage &image, VkFormat format, VkImageAspectFlags aspect);

private:
  const VulkanDevice &m_Device;
  VkImageView m_View;

  void cleanup();
};

} // namespace Froth
