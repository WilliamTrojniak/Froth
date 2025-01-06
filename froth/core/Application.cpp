#include <memory>
#include <utility>
#define GLFW_INCLUDE_VULKAN

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
Application::~Application() {}

void Application::Run() {
  while (m_Running) {
    for (std::shared_ptr<Layer> layer : m_LayerStack) {
      layer->onUpdate(0);
    }
    Window::pollEvents();
  }
}

void Application::onEvent(const Event &e) {
  std::cerr << e.ToString() << std::endl;

  for (std::shared_ptr<Layer> layer : m_LayerStack) {
    if (layer->onEvent(e))
      return;
  }

  EventDispatcher dispatcher = EventDispatcher(e);
  dispatcher.dispatch<WindowCloseEvent>(BIND_FUNC(onWindowClose));
}

void Application::pushLayer(std::shared_ptr<Layer> layer) {
  layer->onAttach();
  m_LayerStack.pushLayer(std::move(layer));
}

void Application::pushOverlay(std::shared_ptr<Layer> overlay) {
  overlay->onAttach();
  m_LayerStack.pushOverlay(std::move(overlay));
}

bool Application::onWindowClose(WindowCloseEvent &e) {
  m_Running = false;
  return true;
}

} // namespace Froth
