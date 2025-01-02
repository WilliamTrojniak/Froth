#pragma once
#include "Event.h"
#include <cstddef>

namespace Froth {
class WindowCloseEvent : public Event {
public:
  WindowCloseEvent() {}
  static inline EventType staticEventType() { return EventType::WindowClose; }
  virtual inline EventType eventType() const override {
    return staticEventType();
  }
  virtual inline int categoryFlags() const override {
    return EventCategoryApplication;
  }
};

class WindowResizeEvent : public Event {
public:
  WindowResizeEvent(size_t width, size_t height)
      : m_Width(width), m_Height(height) {}
  static inline EventType staticEventType() { return EventType::WindowClose; }
  virtual inline EventType eventType() const override {
    return staticEventType();
  }
  virtual inline int categoryFlags() const override {
    return EventCategoryApplication;
  }

  size_t width() { return m_Width; }
  size_t height() { return m_Height; }

private:
  size_t m_Width, m_Height;
};

} // namespace Froth
