#include "Renderer.h"
#include "./vulkan/VulkanRenderer.h"
#include <memory>

namespace Froth {

std::unique_ptr<Renderer> Renderer::createRenderer() {
  // TODO: Change based on defines
  return std::unique_ptr<VulkanRenderer>(new VulkanRenderer());
}

} // namespace Froth
