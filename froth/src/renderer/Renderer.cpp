#include "Renderer.h"
#include "src/renderer/vulkan/VulkanRenderer.h"
#include <memory>

namespace Froth {

std::unique_ptr<Renderer> Renderer::create(const Window &window) {
  // TODO: Switch based on defines
  return VulkanRenderer::create(window);
}

} // namespace Froth
