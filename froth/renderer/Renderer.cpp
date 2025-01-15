#include "Renderer.h"
#include "./vulkan/VulkanRenderer.h"
#include <memory>

namespace Froth {

std::unique_ptr<Renderer> Renderer::create() noexcept {
  // TODO: Switch based on defines
  return std::unique_ptr<Renderer>(new VulkanRenderer);
}

} // namespace Froth
