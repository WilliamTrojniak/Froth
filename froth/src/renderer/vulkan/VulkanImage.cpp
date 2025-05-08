#include "VulkanImage.h"
#include "VulkanImageView.h"
#include "src/core/logger/Logger.h"
#include "src/renderer/vulkan/VulkanContext.h"

namespace Froth {

VulkanImage::VulkanImage(const CreateInfo &opts) {

  // TODO: This should all be customizable
  // maybe opts does not need to be its own type
  VkImageCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  createInfo.imageType = VK_IMAGE_TYPE_2D;
  createInfo.extent.width = opts.extent.width;
  createInfo.extent.height = opts.extent.height;
  createInfo.extent.depth = 1;
  createInfo.mipLevels = 1;
  createInfo.arrayLayers = 1;
  createInfo.format = opts.format;
  createInfo.tiling = opts.tiling;
  createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  createInfo.usage = opts.usage;
  createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VulkanContext &vctx = VulkanContext::get();
  if (vkCreateImage(vctx.device(), &createInfo, vctx.allocator(), &m_Image) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create Vulkan Image");
  }

  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(vctx.device(), m_Image, &memRequirements);

  // TODO: Property flag will need to be brought to part of the constructor
  m_Memory = vctx.device().allocateMemory(memRequirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  if (vkBindImageMemory(vctx.device(), m_Image, m_Memory, 0) != VK_SUCCESS) {
    cleanup();
    FROTH_ERROR("Failed to bind Vulkan Image memory");
  }
}

VulkanImage::~VulkanImage() {
  cleanup();
}

VulkanImageView VulkanImage::createView(VkFormat format, VkImageAspectFlags aspect) const {
  return VulkanImageView(*this, format, aspect);
}

void VulkanImage::cleanup() {
  VulkanContext &vctx = VulkanContext::get();
  if (m_Memory) {

    vkFreeMemory(vctx.device(), m_Memory, vctx.allocator());
    m_Memory = nullptr;
    FROTH_DEBUG("Freed Vulkan Image memory")
  }

  if (m_Image) {
    vkDestroyImage(vctx.device(), m_Image, vctx.allocator());
    m_Image = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Image")
  }
}

} // namespace Froth
