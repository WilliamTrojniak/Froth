#pragma once
#include "Event.h"
#include <concepts>
#include <functional>

namespace Froth {
class EventDispatcher {
private:
  const Event &m_Event;
  bool m_IsHandled = false;

public:
  EventDispatcher(const Event &event) : m_Event(event) {}
  bool isHandled() const { return m_IsHandled; }

  template <std::derived_from<Event> T>
  void dispatch(std::function<bool(T &)> func) {
    if (m_Event.eventType() == T::staticEventType()) {
      m_IsHandled = m_IsHandled || func(*(T *)&m_Event);
    }
  }
};
} // namespace Froth
