#include "Window.h"
#include "GLFWWindow.h"
#include <functional>

namespace Froth {

Window::Window(int width, int height) : m_Width(width), m_Height(height) {}

Window *Window::createWindow(int width, int height, const char *title) {
  // FIXME: Handle error while creating
  try {
    return new GLFWWindow(width, height, title);
  } catch (std::runtime_error e) {
    // TODO: Log error
    return nullptr;
  }
}

void Window::pollEvents() { return GLFWWindow::pollEvents(); }

void Window::setEventCallbackFunction(std::function<void(const Event &)> fn) {
  m_EventCallbackFn = fn;
}

void Window::onEvent(const Event &e) {

  if (m_EventCallbackFn) {
    m_EventCallbackFn(e);
  }
}

} // namespace Froth
