#pragma once

#include "core/events/Event.h"
#include "platform/keys/GLFWCodes.h"

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
  inline MouseCode code() const;

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

private:
  double m_X, m_Y;
};

} // namespace Froth
