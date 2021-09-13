#pragma once
#include "Event.h"

namespace Froth
{
	class KeyEvent : public Event
	{
	private:
		U32 m_Keycode;

	public:
		inline U32 getKeyCode() const { return m_Keycode; }

		virtual int getCategoryFlags() const override { return EventCategoryInput | EventCategoryKeyboard; }

	protected:
		KeyEvent(U32 keycode) : m_Keycode(keycode) {};
	};

	class KeyPressedEvent : public KeyEvent
	{
	private:
		bool m_IsRepeat;

	public:
		KeyPressedEvent(U32 keycode, bool isRepeat) : KeyEvent(keycode), m_IsRepeat(isRepeat) {}

		inline bool isRepeat() const { return m_IsRepeat; }

		static EventType getStaticEventType() { return EventType::KeyPressed; }

		virtual EventType getEventType() const override { return getStaticEventType(); }

	};

	class KeyReleasedEvent : public KeyEvent
	{

	public:
		KeyReleasedEvent(U32 keycode) : KeyEvent(keycode) {}

		static EventType getStaticEventType() { return EventType::KeyReleased; }

		virtual EventType getEventType() const override { return getStaticEventType(); }

	};
}