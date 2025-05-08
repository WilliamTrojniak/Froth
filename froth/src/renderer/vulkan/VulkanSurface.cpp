#include "VulkanSurface.h"
#include "src/core/events/ApplicationEvent.h"
#include "src/core/logger/Logger.h"
#include "src/renderer/vulkan/VulkanContext.h"

namespace Froth {

VulkanSurface::VulkanSurface(VulkanSurface &&o)
    : m_Surface(o.m_Surface) {
  o.m_Surface = nullptr;
}

bool VulkanSurface::onFramebufferResize(FramebufferResizeEvent &e) {
  m_Extent.width = e.width();
  m_Extent.height = e.height();

  return false;
}

VulkanSurface::~VulkanSurface() {
  if (m_Surface != nullptr) {
    VulkanContext &vctx = VulkanContext::get();
    vkDestroySurfaceKHR(vctx.instance(), m_Surface, vctx.allocator());
    m_Surface = nullptr;
    FROTH_DEBUG("Destroyed Vulkan surface")
  }
}

} // namespace Froth
