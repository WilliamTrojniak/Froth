#pragma once

#include "vulkan/vulkan_core.h"
#include <cstdint>

namespace Froth {

class Window;
class VulkanSurface {

public:
  VulkanSurface() = default;
  VulkanSurface(VkSurfaceKHR surface, const VkExtent2D &extent)
      : m_Surface(surface), m_Extent(extent) {}
  VulkanSurface(const VulkanSurface &) = delete;
  VulkanSurface(VulkanSurface &&);
  VulkanSurface &operator=(const VulkanSurface &) = delete;
  ~VulkanSurface();

  operator VkSurfaceKHR() const { return m_Surface; }
  const VkExtent2D &extent() const { return m_Extent; }

  void resize(uint32_t width, uint32_t height);

private:
  VkExtent2D m_Extent{};
  VkSurfaceKHR m_Surface = nullptr;
};
} // namespace Froth
