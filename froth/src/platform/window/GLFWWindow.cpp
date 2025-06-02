#include "GLFWWindow.h"
#include "GLFW/glfw3.h"
#include "src/core/events/ApplicationEvent.h"
#include "src/core/events/KeyEvent.h"
#include "src/core/events/MouseEvent.h"
#include "src/core/logger/Logger.h"
#include "src/platform/keys/GLFWCodes.h"
#include "src/platform/keys/Keycodes.h"
#include "src/platform/window/Window.h"
#include "src/renderer/vulkan/VulkanContext.h"
#include "src/renderer/vulkan/VulkanSurface.h"

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
  FROTH_INFO("Initialized GLFW");

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  m_Window = glfwCreateWindow(width, height, title, NULL, NULL);

  if (!m_Window) {
    if (s_InstanceCount == 0) {
      glfwTerminate();
      FROTH_INFO("Terminated GLFW");
    }
    throw std::runtime_error("Failed to create GLFW window");
    return;
  }
  s_InstanceCount++;

  glfwSetWindowUserPointer(m_Window, this);

  // Window Callbacks
  glfwSetWindowCloseCallback(m_Window, GLFWWindow::windowCloseCallback);
  glfwSetWindowSizeCallback(m_Window, GLFWWindow::windowSizeCallback);

  // Framebuffer Callbacks
  glfwSetFramebufferSizeCallback(m_Window, GLFWWindow::framebufferSizeCallback);

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
    FROTH_INFO("Terminated GLFW");
  }
}

void GLFWWindow::pollEvents() { glfwPollEvents(); }

void GLFWWindow::getCursorPos(double &x, double &y) const {
  glfwGetCursorPos(m_Window, &x, &y);
}

void GLFWWindow::getFramebufferSize(uint32_t &width, uint32_t &height) const {
  int w, h;
  glfwGetFramebufferSize(m_Window, &w, &h);
  width = static_cast<uint32_t>(w);
  height = static_cast<uint32_t>(h);
}

const char **GLFWWindow::requiredVulkanExtensions(uint32_t &extensionCount) noexcept {
  return glfwGetRequiredInstanceExtensions(&extensionCount);
}

void GLFWWindow::setCursorMode(Window::CursorMode mode) {
  switch (mode) {
  case Window::CursorMode::NORMAL:
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case Window::CursorMode::CAPTURED:
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
    break;
  case Window::CursorMode::DISABLED:
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
  case Window::CursorMode::HIDDEN:
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    break;
  }
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

void GLFWWindow::windowSizeCallback(int width, int height) {
  m_Width = width;
  m_Height = height;

  onEvent(WindowResizeEvent(width, height));
}

void GLFWWindow::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  auto handler = static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
  handler->framebufferSizeCallback(width, height);
}

void GLFWWindow::framebufferSizeCallback(int width, int height) {
  onEvent(FramebufferResizeEvent(width, height));
}

VulkanSurface GLFWWindow::createVulkanSurface() const {
  VulkanContext &vctx = VulkanContext::get();
  // TODO: Take into account allocation callback
  // TODO: Should Window own the surface?
  VkSurfaceKHR surface;
  if (glfwCreateWindowSurface(vctx.instance(), m_Window, vctx.allocator(), &surface) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create Vulkan surface for GLFW window");
  }
  VkExtent2D e;
  getFramebufferSize(e.width, e.height);
  return VulkanSurface(surface, e);
};

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
