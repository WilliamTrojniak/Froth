#pragma once

#define GLFW_INCLUDE_VULKAN
#include "renderer/Renderer.h"
#include "renderer/vulkan/VulkanDevice.h"
#include <GLFW/glfw3.h>

namespace Froth {
class VulkanRenderer : public Renderer {
  friend class Renderer;

public:
  ~VulkanRenderer() override;
  VulkanRenderer(VulkanRenderer const &) = delete;
  void operator=(VulkanRenderer const &) = delete;

  static bool init(std::shared_ptr<Window> &window) noexcept;

protected:
  static VulkanRenderer &getInstance();

private:
  VulkanRenderer() = default;
  struct VulkanRendererContext {
    const VkAllocationCallbacks *allocator;
    VkInstance instance;
    VkSurfaceKHR surface;
    std::shared_ptr<const Window> window;
    VulkanDevice device;
  } m_Context;

  static bool initInstance(const VkAllocationCallbacks *allocator, VulkanRendererContext &context) noexcept;
  void shutdown() noexcept;

  static bool s_Initialized;
  static VulkanRenderer &getInstanceInt() noexcept;
};
} // namespace Froth
