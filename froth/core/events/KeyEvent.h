#pragma once

#include "core/events/Event.h"
#include "platform/keys/Keycodes.h"
#include <cstdint>

namespace Froth {
class KeyEvent : public Event {
private:
  uint32_t m_Keycode;

public:
  inline uint32_t keyCode() const { return m_Keycode; }
  virtual inline int categoryFlags() const override {
    return EventCategoryInput | EventCategoryKeyboard;
  }

protected:
  KeyEvent(uint32_t keycode) : m_Keycode(keycode) {}
};

class KeyPressedEvent : public KeyEvent {
private:
  bool m_IsRepeat;

public:
  KeyPressedEvent(uint32_t keycode, bool isRepeat)
      : KeyEvent(keycode), m_IsRepeat(isRepeat) {}

  inline bool isRepeat() const { return m_IsRepeat; }

  static inline EventType staticEventType() { return EventType::KeyPressed; }
  virtual inline EventType eventType() const override {
    return staticEventType();
  }
};

class KeyReleasedEvent : public KeyEvent {
public:
  KeyReleasedEvent(KeyCode keycode) : KeyEvent(keycode) {}
  static inline EventType staticEventType() { return EventType::KeyReleased; }
  virtual inline EventType eventType() const override {
    return staticEventType();
  }
};
} // namespace Froth
