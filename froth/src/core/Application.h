#pragma once
#include "Layer.h"
#include "LayerStack.h"
#include "events/ApplicationEvent.h"
#include "events/Event.h"
#include "src/platform/window/Window.h"
#include "src/renderer/Renderer.h"
#include <memory>

namespace Froth {

class Application {
private:
  bool m_Running = true;
  std::unique_ptr<Window> m_Window;
  std::unique_ptr<Renderer> m_Renderer;
  LayerStack m_LayerStack;

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
  Renderer &renderer() { return *m_Renderer.get(); }
};
Application *CreateApplication();
} // namespace Froth
