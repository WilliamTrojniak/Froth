#pragma once

#include "Window.h"
#include <GLFW/glfw3.h>
#include <cstdint>

namespace Froth {
class GLFWWindow : public Window {
  friend class Window;

private:
  static uint16_t s_InstanceCount;

  GLFWwindow *m_Window;
  void windowCloseCallback();
  void windowSizeCallback(int width, int height);
  void keyCallback(int key, int scancode, int action, int mods);
  void mouseButtonCallback(int button, int action, int mods);
  void mouseMoveCallback(double x, double y);
  void mouseScrollCallback(double xOffset, double yOffset);
  static void windowCloseCallback(GLFWwindow *window);
  static void windowSizeCallback(GLFWwindow *window, int width, int height);
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
  static void mouseMoveCallback(GLFWwindow *window, double x, double y);
  static void mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset);

protected:
  GLFWWindow(int width, int height, const char *title);

public:
  static void pollEvents();
  virtual void *nativeWindow() const override { return m_Window; }
  virtual void getFramebufferSize(uint32_t &width, uint32_t &height) const override;
  static const char **requiredVulkanExtensions(uint32_t &extensionCount) noexcept;
  virtual bool createVulkanSurface(VkInstance instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR &surface) const override;
  ~GLFWWindow() override;
};
} // namespace Froth
