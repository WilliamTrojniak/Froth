#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/Application.h"
#include "core/events/ApplicationEvent.h"
#include "core/events/Event.h"
#include "core/events/EventDispatcher.h"
#include "platform/window/Window.h"
#include <functional>
#include <iostream>

namespace Froth {

#define BIND_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

Application::Application() {

  m_Window = Window::createWindow(640, 480, "Hello World");
  if (!m_Window) {
    m_Running = false;
    return;
  }
  m_Window->setEventCallbackFunction(BIND_FUNC(onEvent));
}
Application::~Application() {
  delete m_Window;
  glfwTerminate();
}

void Application::Run() {
  while (m_Running) {
    Window::pollEvents();
  }
}

void Application::onEvent(const Event &e) {
  EventDispatcher dispatcher = EventDispatcher(e);
  std::cerr << e.ToString() << std::endl;
  dispatcher.dispatch<WindowCloseEvent>(BIND_FUNC(onWindowClose));
}

bool Application::onWindowClose(WindowCloseEvent &e) {
  m_Running = false;
  return true;
}

} // namespace Froth
