#include "VulkanSurface.h"
#include "src/core/logger/Logger.h"
#include "src/platform/window/Window.h"
#include <cstdint>

namespace Froth {

VulkanSurface::VulkanSurface(VulkanSurface &&o)
    : m_Instance(o.m_Instance), m_Surface(o.m_Surface), m_Window(o.m_Window) {
  o.m_Surface = nullptr;
}

VulkanSurface::~VulkanSurface() {
  if (m_Surface != nullptr) {
    vkDestroySurfaceKHR(m_Instance.instance(), m_Surface, m_Instance.allocator());
    m_Surface = nullptr;
    FROTH_DEBUG("Destroyed Vulkan surface")
  }
}

void VulkanSurface::getFramebufferSize(uint32_t &width, uint32_t &height) const {
  m_Window.getFramebufferSize(width, height);
}
} // namespace Froth
