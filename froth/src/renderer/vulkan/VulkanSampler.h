#pragma once

#include <vulkan/vulkan.h>

namespace Froth {
class VulkanSampler {
public:
  VulkanSampler() = default;
  VulkanSampler(const VkSamplerCreateInfo &info);
  VulkanSampler(const VulkanSampler &) = delete;
  VulkanSampler &operator=(const VulkanSampler &) = delete;
  VulkanSampler(VulkanSampler &&) noexcept;
  VulkanSampler &operator=(VulkanSampler &&) noexcept;
  void cleanup();
  ~VulkanSampler() { cleanup(); }

  operator VkSampler() const { return m_Sampler; }

private:
  VkSampler m_Sampler = nullptr;

public:
  class Builder {
  public:
    Builder();
    VulkanSampler build();

  public:
    VkSamplerCreateInfo m_Info;
  };
};

} // namespace Froth
