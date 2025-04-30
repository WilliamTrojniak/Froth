#include "VulkanImageView.h"
#include "VulkanRenderer.h"
#include "src/core/logger/Logger.h"

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

  if (vkCreateImageView(VulkanRenderer::context().device, &createInfo, VulkanRenderer::context().instance.allocator(), &m_View) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create Image View")
  }
}

VulkanImageView::VulkanImageView(VulkanImageView &&other)
    : m_Format(other.m_Format), m_View(other.m_View) {
  other.m_View = nullptr;
}

VulkanImageView::~VulkanImageView() {
  cleanup();
}

void VulkanImageView::cleanup() {
  if (m_View) {
    vkDestroyImageView(VulkanRenderer::context().device, m_View, VulkanRenderer::context().instance.allocator());
    m_View = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Image View")
  }
}

} // namespace Froth
