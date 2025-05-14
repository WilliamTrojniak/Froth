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
  createInfo.extent.depth = opts.extent.depth;
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
  m_Extent = opts.extent;

  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(vctx.device(), m_Image, &memRequirements);

  m_Memory = vctx.device().allocateMemory(memRequirements, opts.memPropFlags);
  if (vkBindImageMemory(vctx.device(), m_Image, m_Memory, 0) != VK_SUCCESS) {
    cleanup();
    FROTH_ERROR("Failed to bind Vulkan Image memory");
  }
}

VulkanImage::VulkanImage(VulkanImage &&o)
    : m_Image(o.m_Image),
      m_Extent(o.m_Extent),
      m_Memory(o.m_Memory) {
  o.m_Image = nullptr;
  o.m_Extent = {};
  o.m_Memory = nullptr;
}

VulkanImage &VulkanImage::operator=(VulkanImage &&o) {
  m_Image = o.m_Image;
  m_Extent = o.m_Extent;
  m_Memory = o.m_Memory;
  o.m_Image = nullptr;
  o.m_Extent = {};
  o.m_Memory = nullptr;

  return *this;
}

VulkanImage::~VulkanImage() {
  cleanup();
}

// HACK: Improvements needed
bool VulkanImage::transitionLayout(VulkanCommandBuffer &commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout) {
  if (!commandBuffer.beginSingleTime())
    return false;

  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = m_Image;
  barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  VkPipelineStageFlags sourceStage, destinationStage;
  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  } else {
    FROTH_ERROR("Unsupported layout transition");
  }
  vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
  if (!commandBuffer.end()) {
    return false;
  }

  VkSubmitInfo submitInfo{};
  VkCommandBuffer b = commandBuffer;
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &b;
  VulkanContext &vctx = VulkanContext::get();
  if (vkQueueSubmit(vctx.device().getQueueFamilies().graphics.queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
    FROTH_WARN("Failed to submit to queue");
    return false;
  }

  // FIXME: Requires copies are done sequentially
  if (vkQueueWaitIdle(vctx.device().getQueueFamilies().graphics.queue) != VK_SUCCESS) {
    FROTH_WARN("Failed to wait for queue idle");
    return false;
  }

  return true;
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
