#pragma once

#include "core/events/Event.h"
#include "platform/keys/Keycodes.h"

namespace Froth {
class KeyEvent : public Event {
private:
  KeyCode m_Keycode;

public:
  inline KeyCode keyCode() const { return m_Keycode; }
  virtual inline int categoryFlags() const override {
    return EventCategoryInput | EventCategoryKeyboard;
  }

protected:
  KeyEvent(KeyCode keycode) : m_Keycode(keycode) {}
};

class KeyPressedEvent : public KeyEvent {
private:
  bool m_IsRepeat;

public:
  KeyPressedEvent(KeyCode keycode, bool isRepeat)
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
