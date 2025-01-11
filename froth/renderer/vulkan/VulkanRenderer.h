#pragma once

#define GLFW_INCLUDE_VULKAN
#include "renderer/Renderer.h"
#include <GLFW/glfw3.h>

namespace Froth {
class VulkanRenderer : public Renderer {
  friend class Renderer;

public:
  ~VulkanRenderer() override;

protected:
  VulkanRenderer();

private:
  bool init() noexcept;
  bool shutdown() noexcept;

  static bool initInstance(VkInstance &instance) noexcept;

  static struct VulkanRendererContext {
    const VkAllocationCallbacks *allocator;
    VkInstance instance;

  } s_Context;

  static uint32_t s_Count;
  static bool s_Initialized;
};
} // namespace Froth
