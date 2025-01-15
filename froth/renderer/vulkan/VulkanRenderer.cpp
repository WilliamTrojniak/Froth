#include "VulkanRenderer.h"
#include "Defines.h"
#include "core/logger/Logger.h"
#include "renderer/vulkan/VulkanDevice.h"
#include "renderer/vulkan/VulkanInstance.h"
#include <exception>
#include <memory>

namespace Froth {

bool hasExtensions(const std::vector<const char *> &extensions) noexcept;
bool getRequiredExtensions(std::vector<const char *> &extensions) noexcept;
bool hasLayers(const std::vector<const char *> &layers) noexcept;

bool VulkanRenderer::s_Initialized = false;
VulkanInstance VulkanRenderer::s_Ctx{};

VulkanRenderer::VulkanRenderer() {
}

VulkanRenderer::~VulkanRenderer() {
  shutdown();
}

bool VulkanRenderer::init(const Window &window) noexcept {
  if (s_Initialized)
    return true;

  try {
    s_Ctx = VulkanInstance(nullptr); // TODO: Configurable allocator
    m_Surface = window.createVulkanSurface(s_Ctx);
    m_Device = std::make_unique<VulkanDevice>(s_Ctx, *m_Surface);
  } catch (std::exception e) {
    return false;
  }

  FROTH_INFO("Initialized Vulkan Renderer")
  s_Initialized = true;
  return true;
}

void VulkanRenderer::shutdown() noexcept {
  s_Initialized = false;
}

} // namespace Froth
