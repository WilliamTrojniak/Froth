#include "VulkanFramebuffer.h"
#include "VulkanRenderer.h"
#include "src/core/logger/Logger.h"

namespace Froth {

VulkanFramebuffer::VulkanFramebuffer(const VulkanRenderPass &renderPass, const VkExtent2D &extent, const std::vector<VkImageView> &attachments) {

  VkFramebufferCreateInfo framebufferInfo{};
  framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  framebufferInfo.renderPass = renderPass;
  framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
  framebufferInfo.pAttachments = attachments.data();
  framebufferInfo.width = extent.width;
  framebufferInfo.height = extent.height;
  framebufferInfo.layers = 1;

  if (vkCreateFramebuffer(VulkanRenderer::context().device, &framebufferInfo, VulkanRenderer::context().instance.allocator(), &m_Framebuffer) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create framebuffer")
  }
}

VulkanFramebuffer::VulkanFramebuffer(VulkanFramebuffer &&other) noexcept
    : m_Framebuffer(other.m_Framebuffer) {
  other.m_Framebuffer = nullptr;
}

VulkanFramebuffer::~VulkanFramebuffer() {
  cleanup();
}

void VulkanFramebuffer::cleanup() {
  if (m_Framebuffer) {
    vkDestroyFramebuffer(VulkanRenderer::context().device, m_Framebuffer, VulkanRenderer::context().instance.allocator());
    m_Framebuffer = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Framebuffer")
  }
}

} // namespace Froth
