#include "GLFWWindow.h"
#include "GLFW/glfw3.h"
#include "core/events/ApplicationEvent.h"
#include "platform/window/Window.h"
#include <IOKit/IOKitLib.h>

namespace Froth {

GLFWWindow::GLFWWindow(int width, int height, const char *title) : Window() {
  m_Window = glfwCreateWindow(width, height, title, NULL, NULL);

  if (!m_Window) {
    // FIXME: Return indicator that creation failed
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
