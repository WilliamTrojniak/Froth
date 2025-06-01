#pragma once

#include "VulkanBuffer.h"
#include "VulkanImageView.h"
#include "VulkanSampler.h"
#include <vector>
#include <vulkan/vulkan.h>

namespace Froth {

class VulkanDescriptorSet {
public:
  class Writer {
  public:
    Writer();
    Writer &addUniform(const VkDescriptorSet dstSet, uint32_t dstBinding, const VulkanBuffer &buffer, uint32_t arrayElement);
    Writer &addImageSampler(const VkDescriptorSet dstSet, uint32_t dstBinding, const VulkanImageView &imageView, const VulkanSampler &sampler, uint32_t arrayElement);
    void Write();

  private:
    Writer &addWrite(const VkDescriptorSet dstSet, uint32_t dstBinding, VkDescriptorType type, VkDescriptorBufferInfo *bufferInfo, VkDescriptorImageInfo *imageInfo, uint32_t arrayElement);
    std::vector<VkWriteDescriptorSet> m_Writes;
    std::vector<VkDescriptorBufferInfo> m_BuffInfos;
    std::vector<VkDescriptorImageInfo> m_ImageInfos;
  };
};

} // namespace Froth
