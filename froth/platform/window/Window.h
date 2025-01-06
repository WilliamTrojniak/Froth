#pragma once

#include "core/events/Event.h"
#include <functional>
#include <memory>

namespace Froth {
class Window {
protected:
  std::function<void(const Event &)> m_EventCallbackFn;
  void onEvent(const Event &e);

  size_t m_Width, m_Height;
  Window(size_t width, size_t height);

public:
  virtual ~Window() = default;
  void setEventCallbackFunction(const std::function<void(const Event &)> &);

  size_t width() const { return m_Width; }
  size_t height() const { return m_Height; }

  virtual void *nativeWindow() const = 0;

  static std::unique_ptr<Window> createWindow(int width, int height, const char *title);

  static void pollEvents();
};
} // namespace Froth
