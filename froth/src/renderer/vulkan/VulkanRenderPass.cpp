#include "VulkanRenderPass.h"
#include "VulkanContext.h"
#include "src/core/logger/Logger.h"
#include <cstddef>

namespace Froth {

VulkanRenderPass::VulkanRenderPass(VkFormat swapchainFormat, VkFormat depthImageViewFormat) {

  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = swapchainFormat;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentRef{};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription depthAttachment{};
  depthAttachment.format = depthImageViewFormat;
  depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depthAttachmentRef{};
  depthAttachmentRef.attachment = 1;
  depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;
  subpass.pDepthStencilAttachment = &depthAttachmentRef;

  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

  VkRenderPassCreateInfo renderPassInfo{};
  std::vector<VkAttachmentDescription> attachments = {colorAttachment, depthAttachment};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
  renderPassInfo.pAttachments = attachments.data();
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;

  VulkanContext &vctx = VulkanContext::get();
  if (vkCreateRenderPass(vctx.device(), &renderPassInfo, vctx.allocator(), &m_RenderPass) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create Render Pass")
  }
}

VulkanRenderPass::VulkanRenderPass(VulkanRenderPass &&o) noexcept
    : m_RenderPass(o.m_RenderPass) {
  o.m_RenderPass = nullptr;
}
VulkanRenderPass &VulkanRenderPass::operator=(VulkanRenderPass &&o) noexcept {
  m_RenderPass = o.m_RenderPass;
  o.m_RenderPass = nullptr;

  return *this;
}

VulkanRenderPass::~VulkanRenderPass() {
  cleanup();
}

void VulkanRenderPass::cleanup() {
  if (m_RenderPass) {
    VulkanContext &vctx = VulkanContext::get();
    vkDestroyRenderPass(vctx.device(), m_RenderPass, vctx.allocator());
    m_RenderPass = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Render Pass")
  }
}

} // namespace Froth
