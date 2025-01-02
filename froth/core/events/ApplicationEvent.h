#pragma once
#include "Event.h"

namespace Froth {
class WindowCloseEvent : public Event {
public:
  WindowCloseEvent() {}
  static EventType staticEventType() { return EventType::WindowClose; }
  virtual EventType eventType() const override { return staticEventType(); }
  virtual int categoryFlags() const override {
    return EventCategoryApplication;
  }
};

} // namespace Froth
