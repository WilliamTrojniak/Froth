#pragma once

#include "renderer/vulkan/VulkanInstance.h"
namespace Froth {

class Window;

class VulkanSurface {

public:
  VulkanSurface(const VulkanInstance &instance, const Window &window, VkSurfaceKHR surface)
      : m_Instance(instance), m_Surface(surface), m_Window(window) {}
  VulkanSurface(const VulkanSurface &) = delete;
  VulkanSurface(VulkanSurface &&);
  VulkanSurface &operator=(const VulkanSurface &) = delete;
  ~VulkanSurface();
  operator VkSurfaceKHR() const { return m_Surface; }

  void getFramebufferSize(uint32_t &width, uint32_t &height) const;

private:
  const VulkanInstance &m_Instance;
  const Window &m_Window;
  VkSurfaceKHR m_Surface;
};
} // namespace Froth
