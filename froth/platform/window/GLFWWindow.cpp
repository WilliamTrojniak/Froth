#include "GLFWWindow.h"
#include "GLFW/glfw3.h"
#include "core/events/ApplicationEvent.h"
#include "platform/window/Window.h"
#include <IOKit/IOKitLib.h>
#include <stdexcept>

namespace Froth {

GLFWWindow::GLFWWindow(int width, int height, const char *title) : Window() {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
    return;
  }

  m_Window = glfwCreateWindow(width, height, title, NULL, NULL);

  if (!m_Window) {
    throw std::runtime_error("Failed to create GLFW window");
    return;
  }

  glfwSetWindowUserPointer(m_Window, this);

  glfwSetWindowCloseCallback(m_Window, GLFWWindow::windowCloseCallback);
}

GLFWWindow::~GLFWWindow() { glfwDestroyWindow(m_Window); }

void GLFWWindow::pollEvents() { glfwPollEvents(); }

void GLFWWindow::windowCloseCallback(GLFWwindow *window) {
  auto handler = static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
  handler->windowCloseCallback();
}
void GLFWWindow::windowCloseCallback() { onEvent(WindowCloseEvent()); }

} // namespace Froth
