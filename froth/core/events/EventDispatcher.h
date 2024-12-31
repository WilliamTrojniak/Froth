#pragma once
#include "Event.h"
#include <concepts>
#include <functional>

namespace Froth {
class EventDispatcher {
private:
  Event &m_Event;

public:
  EventDispatcher(Event &event) : m_Event(event) {}

  template <std::derived_from<Event> T>
  bool dispatch(std::function<bool(T &)> func) {
    if (m_Event.eventType() == T::staticEventType()) {
      m_Event.isHandled = func(*(T *)&m_Event);
      return true;
    }
    return false;
  }
};
} // namespace Froth
