#pragma once

#include "core/events/Event.h"
#include <functional>

namespace Froth {
class Window {
protected:
  std::function<void(const Event &)> m_EventCallbackFn;
  void onEvent(const Event &e);

public:
  void setEventCallbackFunction(std::function<void(const Event &)>);

  static Window *createWindow(int width, int height, const char *title);

  static void pollEvents();
};
} // namespace Froth
