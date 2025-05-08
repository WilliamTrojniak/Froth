#pragma once
#include "vulkan/vulkan_core.h"

namespace Froth {
class VulkanImageView;

class VulkanImage {
  friend class VulkanRenderer;

public:
  struct CreateInfo;

  VulkanImage() = default;
  VulkanImage(const CreateInfo &opts);
  VulkanImage(VulkanImage const &) = delete;
  VulkanImage &operator=(VulkanImage const &) = delete;
  VulkanImage(VulkanImage &&);
  VulkanImage &operator=(VulkanImage &&);
  ~VulkanImage();

  VkImage image() const { return m_Image; }
  void cleanup();
  VulkanImageView createView(VkFormat format, VkImageAspectFlags aspect) const;

  struct CreateInfo {
    VkExtent2D extent;
    VkFormat format;
    VkImageTiling tiling;
    VkImageUsageFlags usage;
  };

private:
  VkImage m_Image = nullptr;
  VkDeviceMemory m_Memory = nullptr;
};

} // namespace Froth
