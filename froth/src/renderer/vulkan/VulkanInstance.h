#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

namespace Froth {
class VulkanInstance {
public:
  VulkanInstance() : m_Instance(VK_NULL_HANDLE) {};
  VulkanInstance(const VkAllocationCallbacks *allocator);
  ~VulkanInstance();

  VulkanInstance(VulkanInstance const &) = delete;
  void operator=(VulkanInstance const &) = delete;
  void operator=(VulkanInstance &&);
  operator VkInstance() const noexcept { return m_Instance; };
  const VkAllocationCallbacks *allocator() const noexcept { return m_Allocator; }

private:
  const VkAllocationCallbacks *m_Allocator;
  VkInstance m_Instance;
};

} // namespace Froth
