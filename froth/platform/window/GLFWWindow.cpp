#include "GLFWWindow.h"
#include "GLFW/glfw3.h"
#include "core/events/ApplicationEvent.h"
#include "core/events/KeyEvent.h"
#include "platform/keys/GLFWKeycodes.h"
#include "platform/keys/Keycodes.h"
#include "platform/window/Window.h"
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
  glfwSetKeyCallback(m_Window, GLFWWindow::keyCallback);
}

GLFWWindow::~GLFWWindow() { glfwDestroyWindow(m_Window); }

void GLFWWindow::pollEvents() { glfwPollEvents(); }

void GLFWWindow::windowCloseCallback(GLFWwindow *window) {
  auto handler = static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
  handler->windowCloseCallback();
}
void GLFWWindow::windowCloseCallback() { onEvent(WindowCloseEvent()); }

void GLFWWindow::keyCallback(GLFWwindow *window, int key, int scancode,
                             int action, int mods) {
  auto handler = static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
  handler->keyCallback(key, scancode, action, mods);
}

void GLFWWindow::keyCallback(int key, int scancode, int action, int mods) {

  KeyCode k = GLFWKeys.contains(key) ? GLFWKeys.at(key) : 0;
  switch (action) {
  case 0:
    onEvent(KeyReleasedEvent(k));
    break;
  case 1:
    onEvent(KeyPressedEvent(k, false));
    break;
  case 2:
    onEvent(KeyPressedEvent(k, true));
    break;
  }
}

} // namespace Froth
