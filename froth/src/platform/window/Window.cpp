#include "Window.h"
#include "GLFWWindow.h"
#include <functional>
#include <memory>

namespace Froth {

Window::Window(size_t width, size_t height)
    : m_Width(width), m_Height(height) {}

std::unique_ptr<Window> Window::createWindow(int width, int height, const char *title) {
  // FIXME: Handle error while creating
  try {
    return std::unique_ptr<GLFWWindow>(new GLFWWindow(width, height, title));
  } catch (std::runtime_error e) {
    // TODO: Log error
    return nullptr;
  }
}

const char **Window::requiredVulkanExtensions(uint32_t &extensionCount) noexcept {
  // TODO: Switch based on Window API
  return GLFWWindow::requiredVulkanExtensions(extensionCount);
}

void Window::pollEvents() { return GLFWWindow::pollEvents(); }

void Window::setEventCallbackFunction(
    const std::function<void(const Event &)> &fn) {
  m_EventCallbackFn = fn;
}

void Window::onEvent(const Event &e) {

  if (m_EventCallbackFn) {
    m_EventCallbackFn(e);
  }
}

} // namespace Froth
