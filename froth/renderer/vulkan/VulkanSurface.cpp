#include "VulkanSurface.h"
#include "core/logger/Logger.h"

namespace Froth {

VulkanSurface::~VulkanSurface() {
  if (m_Surface != nullptr) {
    vkDestroySurfaceKHR(m_Instance.instance(), m_Surface, m_Instance.allocator());
    m_Surface = nullptr;
    FROTH_DEBUG("Destroyed Vulkan surface")
  }
}
} // namespace Froth
