#include "GLFWWindow.h"
#include "GLFW/glfw3.h"
#include "core/events/ApplicationEvent.h"
#include "core/events/KeyEvent.h"
#include "core/events/MouseEvent.h"
#include "core/logger/Logger.h"
#include "platform/keys/GLFWCodes.h"
#include "platform/keys/Keycodes.h"
#include "platform/window/Window.h"
#include <cstdint>
#include <stdexcept>

namespace Froth {

uint16_t GLFWWindow::s_InstanceCount = 0;

GLFWWindow::GLFWWindow(int width, int height, const char *title)
    : Window(width, height) {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
    return;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  m_Window = glfwCreateWindow(width, height, title, NULL, NULL);

  if (!m_Window) {
    if (s_InstanceCount == 0) {
      glfwTerminate();
    }
    throw std::runtime_error("Failed to create GLFW window");
    return;
  }
  s_InstanceCount++;

  glfwSetWindowUserPointer(m_Window, this);

  // Window Callbacks
  glfwSetWindowCloseCallback(m_Window, GLFWWindow::windowCloseCallback);
  glfwSetWindowSizeCallback(m_Window, GLFWWindow::windowSizeCallback);

  // Keyboard Callbacks
  glfwSetKeyCallback(m_Window, GLFWWindow::keyCallback);

  // Mouse Callbacks
  glfwSetMouseButtonCallback(m_Window, GLFWWindow::mouseButtonCallback);
  glfwSetScrollCallback(m_Window, GLFWWindow::mouseScrollCallback);
  glfwSetCursorPosCallback(m_Window, GLFWWindow::mouseMoveCallback);

  // TODO: Implement
  glfwSetCharCallback(m_Window, NULL);
  glfwSetDropCallback(m_Window, NULL);
}

GLFWWindow::~GLFWWindow() {
  glfwDestroyWindow(m_Window);
  s_InstanceCount--;
  if (s_InstanceCount == 0) {
    glfwTerminate();
  }
}

void GLFWWindow::pollEvents() { glfwPollEvents(); }

void GLFWWindow::getFramebufferSize(uint32_t &width, uint32_t &height) const {
  int w, h;
  glfwGetFramebufferSize(m_Window, &w, &h);
  width = static_cast<uint32_t>(w);
  height = static_cast<uint32_t>(h);
}

const char **GLFWWindow::requiredVulkanExtensions(uint32_t &extensionCount) noexcept {
  return glfwGetRequiredInstanceExtensions(&extensionCount);
}
void GLFWWindow::windowCloseCallback(GLFWwindow *window) {
  auto handler = static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
  handler->windowCloseCallback();
}
void GLFWWindow::windowCloseCallback() { onEvent(WindowCloseEvent()); }

void GLFWWindow::windowSizeCallback(GLFWwindow *window, int width, int height) {
  auto handler = static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
  handler->windowSizeCallback(width, height);
}

bool GLFWWindow::createVulkanSurface(VkInstance instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR &surface) const {
  // TODO: Take into account allocation callback
  // TODO: Should Window own the surface?
  if (glfwCreateWindowSurface(instance, m_Window, allocator, &surface) != VK_SUCCESS) {
    FROTH_WARN("Failed to create Vulkan surface for GLFW window");
    return false;
  }
  return true;
};

void GLFWWindow::windowSizeCallback(int width, int height) {
  m_Width = width;
  m_Height = height;

  onEvent(WindowResizeEvent(width, height));
}

void GLFWWindow::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  auto handler = static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
  handler->keyCallback(key, scancode, action, mods);
}

void GLFWWindow::keyCallback(int key, int scancode, int action, int mods) {

  KeyCode k = GLFWKeyCodes.contains(key) ? GLFWKeyCodes.at(key) : 0;
  switch (action) {
  case GLFW_RELEASE:
    onEvent(KeyReleasedEvent(k));
    break;
  case GLFW_PRESS:
    onEvent(KeyPressedEvent(k, false));
    break;
  case GLFW_REPEAT:
    onEvent(KeyPressedEvent(k, true));
    break;
  }
}

void GLFWWindow::mouseButtonCallback(GLFWwindow *window, int button, int action,
                                     int mods) {
  auto handler = static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
  handler->mouseButtonCallback(button, action, mods);
}

void GLFWWindow::mouseButtonCallback(int button, int action, int mods) {
  MouseCode code = GLFWMouseCodes.contains(button) ? GLFWMouseCodes.at(button) : 0;

  switch (action) {
  case GLFW_PRESS:
    onEvent(MouseButtonPressedEvent(code));
    break;
  case GLFW_REPEAT:
    onEvent(MouseButtonReleasedEvent(code));
  }
}

void GLFWWindow::mouseMoveCallback(GLFWwindow *window, double x, double y) {
  auto handler = static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
  handler->mouseMoveCallback(x, y);
}

void GLFWWindow::mouseMoveCallback(double x, double y) {
  onEvent(MouseMoveEvent(x, y));
}

void GLFWWindow::mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
  auto handler = static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
  handler->mouseScrollCallback(xOffset, yOffset);
}

void GLFWWindow::mouseScrollCallback(double xOffset, double yOffset) {
  onEvent(MouseScrollEvent(xOffset, yOffset));
}

} // namespace Froth
