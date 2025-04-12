#pragma once

#include "renderer/vulkan/VulkanInstance.h"
namespace Froth {

class VulkanSurface {
public:
  VulkanSurface(const VulkanInstance &instance, VkSurfaceKHR surface)
      : m_Instance(instance), m_Surface(surface) {}
  VulkanSurface(const VulkanSurface &) = delete;
  VulkanSurface(VulkanSurface &&);
  VulkanSurface &operator=(const VulkanSurface &) = delete;
  ~VulkanSurface();
  VkSurfaceKHR surface() const { return m_Surface; }

private:
  const VulkanInstance &m_Instance;
  VkSurfaceKHR m_Surface;
};
} // namespace Froth
