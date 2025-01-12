#include "Renderer.h"
#include "./vulkan/VulkanRenderer.h"

namespace Froth {

Renderer &Renderer::getInstance() noexcept {
  // TODO: Change based on defines
  return VulkanRenderer::getInstance();
}

bool Renderer::init(std::shared_ptr<Window> &window) noexcept {
  // TODO: Change based on defines
  return VulkanRenderer::init(window);
}

void Renderer::shutdown() noexcept {
  // TODO: Change based on defines
  VulkanRenderer::getInstance().shutdown();
}

} // namespace Froth
