#pragma once

#include "core/events/Event.h"
#include "platform/keys/GLFWCodes.h"
#include <sstream>

namespace Froth {
class MouseEvent : public Event {
public:
  virtual inline int categoryFlags() const override {
    return EventCategoryInput | EventCategoryKeyboard;
  }

protected:
  MouseEvent() = default;
};

class MouseButtonEvent : public MouseEvent {
public:
  inline MouseCode button() const { return m_Code; }

private:
  MouseCode m_Code;

protected:
  MouseButtonEvent(MouseCode code) : MouseEvent(), m_Code(code) {}
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
  MouseButtonPressedEvent(MouseCode code) : MouseButtonEvent(code) {}
  static inline EventType staticEventType() {
    return EventType::MouseButtonPressed;
  }
  virtual inline EventType eventType() const override {
    return staticEventType();
  }
  virtual std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonPressedEvent: " << button();
    return ss.str();
  }
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
  MouseButtonReleasedEvent(MouseCode code) : MouseButtonEvent(code) {}
  static inline EventType staticEventType() {
    return EventType::MouseButtonReleased;
  }
  virtual inline EventType eventType() const override {
    return staticEventType();
  }
  virtual std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonReleasedEvent: " << button();
    return ss.str();
  }
};

class MouseMoveEvent : public MouseEvent {
public:
  MouseMoveEvent(double x, double y) : MouseEvent(), m_X(x), m_Y(y) {}
  double x() const { return m_X; }
  double y() const { return m_Y; }

  static inline EventType staticEventType() { return EventType::MouseMove; }
  virtual inline EventType eventType() const override {
    return staticEventType();
  }
  virtual std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseMoveEvent: " << m_X << ", " << m_Y;
    return ss.str();
  }

private:
  double m_X, m_Y;
};
class MouseScrollEvent : public MouseEvent {
public:
  MouseScrollEvent(double xOffset, double yOffset)
      : MouseEvent(), m_X(xOffset), m_Y(yOffset) {}
  double xOffset() const { return m_X; }
  double yOffset() const { return m_Y; }

  static inline EventType staticEventType() { return EventType::MouseScroll; }
  virtual inline EventType eventType() const override {
    return staticEventType();
  }
  virtual std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseScrollEvent: " << m_X << ", " << m_Y;
    return ss.str();
  }

private:
  double m_X, m_Y;
};

} // namespace Froth
