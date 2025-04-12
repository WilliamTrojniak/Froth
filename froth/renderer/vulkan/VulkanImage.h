#pragma once
#include "VulkanDevice.h"

namespace Froth {
class VulkanImageView;

class VulkanImage {
  friend class VulkanRenderer;

public:
  VulkanImage(VulkanImage const &) = delete;
  void operator=(VulkanImage const &) = delete;
  ~VulkanImage();
  VkImage image() const { return m_Image; }
  VulkanImageView createView(VkFormat format, VkImageAspectFlags aspect) const;

  struct CreateInfo {
    struct {
      uint32_t width;
      uint32_t height;
    } extent;
    VkFormat format;
    VkImageTiling tiling;
    VkImageUsageFlags usage;
  };

protected:
  VulkanImage(const VulkanDevice &device, const CreateInfo &opts);

private:
  const VulkanDevice &m_Device;
  VkImage m_Image;
  VkDeviceMemory m_Memory;

  void cleanup();
};

} // namespace Froth
