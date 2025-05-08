#include "VulkanSurface.h"
#include "src/core/logger/Logger.h"
#include "src/renderer/vulkan/VulkanContext.h"
#include <cstdint>

namespace Froth {

VulkanSurface::VulkanSurface(VulkanSurface &&o)
    : m_Surface(o.m_Surface) {
  o.m_Surface = nullptr;
}

void VulkanSurface::resize(uint32_t width, uint32_t height) {
  m_Extent.width = width;
  m_Extent.height = height;
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
