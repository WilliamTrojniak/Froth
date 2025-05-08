#pragma once

#include "src/core/events/ApplicationEvent.h"
#include "vulkan/vulkan_core.h"

namespace Froth {

class Window;
class VulkanSurface {

public:
  VulkanSurface(VkSurfaceKHR surface, const VkExtent2D &extent)
      : m_Surface(surface), m_Extent(extent) {}
  VulkanSurface(const VulkanSurface &) = delete;
  VulkanSurface(VulkanSurface &&);
  VulkanSurface &operator=(const VulkanSurface &) = delete;
  ~VulkanSurface();
  operator VkSurfaceKHR() const { return m_Surface; }
  bool onFramebufferResize(FramebufferResizeEvent &e);
  const VkExtent2D &extent() const { return m_Extent; }

private:
  VkSurfaceKHR m_Surface;
  VkExtent2D m_Extent;
};
} // namespace Froth
