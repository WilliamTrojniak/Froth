#include "VulkanDescriptorSet.h"
#include "src/renderer/vulkan/VulkanContext.h"
#include "src/renderer/vulkan/VulkanImageView.h"
#include <vulkan/vulkan_core.h>

namespace Froth {

// FIXME: Pointers can change
VulkanDescriptorSet::Writer::Writer()
    : m_BuffInfos(4), m_ImageInfos(4) {}

VulkanDescriptorSet::Writer &VulkanDescriptorSet::Writer::addWrite(const VkDescriptorSet dstSet, uint32_t dstBinding, VkDescriptorType type, VkDescriptorBufferInfo *bufferInfo, VkDescriptorImageInfo *imageInfo, uint32_t arrayElement) {
  m_Writes.emplace_back(VkWriteDescriptorSet{
      .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .dstSet = dstSet,
      .dstBinding = dstBinding,
      .dstArrayElement = arrayElement, // TODO: Configurable
      .descriptorCount = 1,            // TODO: Configurable
      .descriptorType = type,
      .pImageInfo = imageInfo,
      .pBufferInfo = bufferInfo,
  });

  return *this;
}

VulkanDescriptorSet::Writer &VulkanDescriptorSet::Writer::addUniform(const VkDescriptorSet dstSet, uint32_t dstBinding, const VulkanBuffer &buffer, uint32_t arrayElement) {
  // TODO: Configurable
  m_BuffInfos.emplace_back(VkDescriptorBufferInfo{
      .buffer = buffer,
      .offset = 0,
      .range = buffer.size(),
  });

  return addWrite(dstSet, dstBinding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &m_BuffInfos[m_BuffInfos.size() - 1], VK_NULL_HANDLE, arrayElement);
}
VulkanDescriptorSet::Writer &VulkanDescriptorSet::Writer::addImageSampler(const VkDescriptorSet dstSet, uint32_t dstBinding, const VulkanImageView &imageView, const VulkanSampler &sampler, uint32_t arrayElement) {
  // TODO: Configurable
  m_ImageInfos.emplace_back(VkDescriptorImageInfo{
      .sampler = sampler,
      .imageView = imageView,
      .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, // TODO: Ensure
  });

  return addWrite(dstSet, dstBinding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_NULL_HANDLE, &m_ImageInfos[m_ImageInfos.size() - 1], arrayElement);
}

void VulkanDescriptorSet::Writer::Write() {
  VulkanContext &vctx = VulkanContext::get();
  vkUpdateDescriptorSets(vctx.device(), static_cast<uint32_t>(m_Writes.size()), m_Writes.data(), 0, VK_NULL_HANDLE);
  m_Writes.clear();
  m_BuffInfos.clear();
  m_ImageInfos.clear();
}

} // namespace Froth
