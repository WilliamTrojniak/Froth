#pragma once

#include "VulkanInstance.h"

namespace Froth {

class Window;
class VulkanSurface {

public:
  VulkanSurface(const Window &window, VkSurfaceKHR surface)
      : m_Surface(surface), m_Window(window) {}
  VulkanSurface(const VulkanSurface &) = delete;
  VulkanSurface(VulkanSurface &&);
  VulkanSurface &operator=(const VulkanSurface &) = delete;
  ~VulkanSurface();
  operator VkSurfaceKHR() const { return m_Surface; }

  void getFramebufferSize(uint32_t &width, uint32_t &height) const;

private:
  const Window &m_Window;
  VkSurfaceKHR m_Surface;
};
} // namespace Froth
