#pragma once

#define GLFW_INCLUDE_VULKAN
#include "core/events/Event.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <functional>
#include <memory>

namespace Froth {
class Window {
protected:
  std::function<void(const Event &)> m_EventCallbackFn;
  void onEvent(const Event &e);

  size_t m_Width, m_Height;
  Window(size_t width, size_t height);

public:
  virtual ~Window() = default;
  void setEventCallbackFunction(const std::function<void(const Event &)> &);

  size_t width() const { return m_Width; }
  size_t height() const { return m_Height; }

  virtual void getFramebufferSize(uint32_t &width, uint32_t &height) const = 0;
  static const char **requiredVulkanExtensions(uint32_t &extensionCount) noexcept;
  virtual bool createVulkanSurface(VkInstance instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR &surface) const = 0;

  virtual void *
  nativeWindow() const = 0;

  static std::unique_ptr<Window> createWindow(int width, int height, const char *title);

  static void pollEvents();
};
} // namespace Froth
