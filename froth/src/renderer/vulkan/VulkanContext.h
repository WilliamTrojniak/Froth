#pragma once

#include "src/platform/window/Window.h"
#include "src/renderer/vulkan/VulkanDevice.h"
#include "src/renderer/vulkan/VulkanInstance.h"

namespace Froth {

class VulkanContext {
public:
  ~VulkanContext() { cleanup(); }
  VulkanContext(const VulkanContext &) = delete;
  VulkanContext &operator=(const VulkanContext &) = delete;

  static VulkanContext &get() {
    static VulkanContext s_Instance;
    return s_Instance;
  }

  const VkAllocationCallbacks *allocator() const { return m_Allocator; }
  const VulkanInstance &instance() const { return m_Instance; }
  const VkPhysicalDevice physicalDevice() const { return m_Device.physicalDevice(); }
  const VulkanDevice &device() const { return m_Device; }

  void init(const Window &w);
  void cleanup();

private:
  VulkanContext() = default;

  VkAllocationCallbacks *m_Allocator = nullptr;
  VulkanInstance m_Instance;
  VulkanDevice m_Device;
};

} // namespace Froth
