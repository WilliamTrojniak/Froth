#include "Renderer.h"
#include "./vulkan/VulkanRenderer.h"
#include <memory>

namespace Froth {

std::unique_ptr<Renderer> Renderer::create(const Window &window) {
  // TODO: Switch based on defines
  return VulkanRenderer::create(window);
}

} // namespace Froth
