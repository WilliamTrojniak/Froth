#pragma once
#include "Event.h"
#include <cstddef>
#include <sstream>

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
  virtual std::string ToString() const override {
    std::stringstream ss;
    ss << "WindowCloseEvent";
    return ss.str();
  }
};

class WindowResizeEvent : public Event {
public:
  WindowResizeEvent(size_t width, size_t height)
      : m_Width(width), m_Height(height) {}
  static inline EventType staticEventType() { return EventType::WindowResize; }
  virtual inline EventType eventType() const override {
    return staticEventType();
  }
  virtual inline int categoryFlags() const override {
    return EventCategoryApplication;
  }
  virtual std::string ToString() const override {
    std::stringstream ss;
    ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
    return ss.str();
  }

  size_t width() { return m_Width; }
  size_t height() { return m_Height; }

private:
  size_t m_Width, m_Height;
};

class FramebufferResizeEvent : public Event {
public:
  FramebufferResizeEvent(size_t width, size_t height)
      : m_Width(width), m_Height(height) {}
  static inline EventType staticEventType() { return EventType::FramebufferResize; }
  virtual inline EventType eventType() const override {
    return staticEventType();
  }
  virtual inline int categoryFlags() const override {
    return EventCategoryApplication;
  }
  virtual std::string ToString() const override {
    std::stringstream ss;
    ss << "FramebufferResizeEvent: " << m_Width << ", " << m_Height;
    return ss.str();
  }

  size_t width() { return m_Width; }
  size_t height() { return m_Height; }

private:
  size_t m_Width, m_Height;
};

} // namespace Froth
