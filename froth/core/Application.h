#pragma once
#include "core/Layer.h"
#include "core/LayerStack.h"
#include "events/ApplicationEvent.h"
#include "events/Event.h"
#include "platform/window/Window.h"
#include "renderer/Renderer.h"
#include <memory>

namespace Froth {

class Application {
private:
  bool m_Running = true;
  LayerStack m_LayerStack;
  std::unique_ptr<Window> m_Window;
  std::unique_ptr<Renderer> m_Renderer;

public:
  Application();
  ~Application();
  void Run();

  void pushLayer(std::shared_ptr<Layer> layer);
  void pushOverlay(std::shared_ptr<Layer> overlay);

  void onEvent(const Event &e);
  bool onWindowClose(WindowCloseEvent &e);
  bool onWindowResize(WindowResizeEvent &e);

  // TODO: Remove
  const Window &window() { return *m_Window.get(); }
};
Application *CreateApplication();
} // namespace Froth
