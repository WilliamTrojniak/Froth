#include "VulkanImageView.h"
#include "src/core/logger/Logger.h"
#include "src/renderer/vulkan/VulkanContext.h"

namespace Froth {

VulkanImageView::VulkanImageView(const VulkanImage &image, VkFormat format, VkImageAspectFlags aspect)
    : m_Format(format) {

  // TODO: Make the rest of this config accessible
  VkImageViewCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  createInfo.image = image.image();
  createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  createInfo.format = format;
  createInfo.subresourceRange.aspectMask = aspect;
  createInfo.subresourceRange.baseMipLevel = 0;
  createInfo.subresourceRange.levelCount = 1;
  createInfo.subresourceRange.baseArrayLayer = 0;
  createInfo.subresourceRange.layerCount = 1;

  VulkanContext &vctx = VulkanContext::get();
  if (vkCreateImageView(vctx.device(), &createInfo, vctx.allocator(), &m_View) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create Image View")
  }
}

VulkanImageView::VulkanImageView(VulkanImageView &&other)
    : m_Format(other.m_Format), m_View(other.m_View) {
  other.m_View = nullptr;
  other.m_Format = VK_FORMAT_MAX_ENUM;
}
VulkanImageView &VulkanImageView::operator=(VulkanImageView &&other) {
  m_Format = other.m_Format;
  m_View = other.m_View;
  other.m_View = nullptr;
  other.m_Format = VK_FORMAT_MAX_ENUM;

  return *this;
}

VulkanImageView::~VulkanImageView() {
  cleanup();
}

void VulkanImageView::cleanup() {
  if (m_View) {
    VulkanContext &vctx = VulkanContext::get();
    vkDestroyImageView(vctx.device(), m_View, vctx.allocator());
    m_View = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Image View")
  }
}

} // namespace Froth
