#pragma once
#include "events/ApplicationEvent.h"
#include "events/Event.h"
#include "platform/window/Window.h"

namespace Froth {

class Application {
private:
  bool m_Running = true;
  Window *m_Window;

public:
  Application();
  ~Application();
  void Run();

  void onEvent(const Event &e);
  bool onWindowClose(WindowCloseEvent &e);
};
Application *CreateApplication();
} // namespace Froth
