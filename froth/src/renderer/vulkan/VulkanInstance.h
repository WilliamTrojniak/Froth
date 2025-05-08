#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <memory>

namespace Froth {
class VulkanInstance {
  friend class VulkanContext;

public:
  ~VulkanInstance();

  VulkanInstance(VulkanInstance const &) = delete;
  void operator=(VulkanInstance const &) = delete;
  VulkanInstance(VulkanInstance &&) noexcept;
  VulkanInstance &operator=(VulkanInstance &&) noexcept;

  operator VkInstance() const noexcept { return m_Instance; };

  void cleanup(const VkAllocationCallbacks *allocator);

protected:
  VulkanInstance() = default;
  VulkanInstance(const VkAllocationCallbacks *allocator);

private:
  VkInstance m_Instance = nullptr;
};

} // namespace Froth
