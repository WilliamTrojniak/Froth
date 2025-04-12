#include "VulkanRenderer.h"
#include "Defines.h"
#include "renderer/vulkan/VulkanDevice.h"
#include "renderer/vulkan/VulkanInstance.h"
#include <memory>

namespace Froth {

bool hasExtensions(const std::vector<const char *> &extensions) noexcept;
bool getRequiredExtensions(std::vector<const char *> &extensions) noexcept;
bool hasLayers(const std::vector<const char *> &layers) noexcept;

bool VulkanRenderer::s_Initialized = false;
VulkanInstance VulkanRenderer::s_Ctx{};

VulkanRenderer::VulkanRenderer(const Window &window, VulkanSurface &&surface)
    : m_Surface(std::move(surface)), m_Device(s_Ctx, m_Surface), m_Swapchain(m_Device, window, m_Surface) {
}

VulkanRenderer::~VulkanRenderer() {
  shutdown();
}

std::unique_ptr<VulkanRenderer> VulkanRenderer::create(const Window &window) {
  if (!s_Initialized) {
    s_Ctx = VulkanInstance(nullptr); // TODO: Configurable allocator
    s_Initialized = true;
  }
  return std::unique_ptr<VulkanRenderer>(new VulkanRenderer(window, window.createVulkanSurface(s_Ctx)));
}

void VulkanRenderer::shutdown() noexcept {
  s_Initialized = false;
}

} // namespace Froth
