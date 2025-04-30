#include "VulkanFramebuffer.h"
#include "src/core/logger/Logger.h"

namespace Froth {

VulkanFramebuffer::VulkanFramebuffer(const VulkanDevice &device, const VulkanRenderPass &renderPass, const VkExtent2D &extent, const std::vector<VkImageView> &attachments)
    : m_Device(device) {

  VkFramebufferCreateInfo framebufferInfo{};
  framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  framebufferInfo.renderPass = renderPass;
  framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
  framebufferInfo.pAttachments = attachments.data();
  framebufferInfo.width = extent.width;
  framebufferInfo.height = extent.height;
  framebufferInfo.layers = 1;

  if (vkCreateFramebuffer(device, &framebufferInfo, device.instance().allocator(), &m_Framebuffer) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create framebuffer")
  }
}

VulkanFramebuffer::VulkanFramebuffer(VulkanFramebuffer &&other) noexcept
    : m_Device(other.m_Device) {
  m_Framebuffer = other.m_Framebuffer;
  other.m_Framebuffer = nullptr;
}

VulkanFramebuffer::~VulkanFramebuffer() {
  cleanup();
}

void VulkanFramebuffer::cleanup() {
  if (m_Framebuffer) {
    vkDestroyFramebuffer(m_Device, m_Framebuffer, m_Device.instance().allocator());
    m_Framebuffer = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Framebuffer")
  }
}

} // namespace Froth
