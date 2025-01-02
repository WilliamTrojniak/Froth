#pragma once
#include "Event.h"
#include <concepts>
#include <functional>

namespace Froth {
class EventDispatcher {
private:
  const Event &m_Event;

public:
  EventDispatcher(const Event &event) : m_Event(event) {}

  template <std::derived_from<Event> T>
  bool dispatch(std::function<bool(T &)> func) {
    if (m_Event.eventType() == T::staticEventType()) {
      return func(*(T *)&m_Event);
    }
    return false;
  }
};
} // namespace Froth
