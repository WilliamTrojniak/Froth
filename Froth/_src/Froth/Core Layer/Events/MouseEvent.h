#pragma once
#include "Event.h"

namespace Froth
{
	class MouseButtonEvent : public Event
	{
	private:
		U32 m_Button;

	public:
		inline U32 getMouseButton() const { return m_Button; }

		virtual int getCategoryFlags() const override { return EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton; }

	protected:
		MouseButtonEvent(U32 button) : m_Button(button) {}
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(U32 button) : MouseButtonEvent(button) {}

		static EventType getStaticEventType() { return EventType::MouseButtonPressed; }

		virtual EventType getEventType() const override { return getStaticEventType(); }
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(U32 button) : MouseButtonEvent(button) {}

		static EventType getStaticEventType() { return EventType::MouseButtonReleased; }

		virtual EventType getEventType() const override { return getStaticEventType(); }
	};


	class MouseMoveEvent : public Event
	{
	private:
		struct Position
		{
			F32 x, y;
		} m_Position;	

	public:
		MouseMoveEvent(F32 x, F32 y) : m_Position({ x, y }) {}

		inline Position getPosition() const { return m_Position; }
		inline F32 getX() const { return m_Position.x; }
		inline F32 getY() const { return m_Position.y; }

		static EventType getStaticEventType() { return EventType::MouseMove; }

		virtual EventType getEventType() const override { return getStaticEventType(); }

		virtual int getCategoryFlags() const override { return EventCategoryInput | EventCategoryMouse; }
	};

	class MouseScrollEvent : public Event
	{
	private:
		struct Offset
		{
			F32 x, y;
		} m_Offset;

	public:
		MouseScrollEvent(F32 xOffset, F32 yOffset) : m_Offset({ xOffset, yOffset }) {}

		inline Offset getOffset() const { return m_Offset; }
		inline F32 getX() const { return m_Offset.x; }
		inline F32 getY() const { return m_Offset.y; }

		static EventType getStaticEventType() { return EventType::MouseScroll; }
		virtual EventType getEventType() const override { return getStaticEventType(); }

		virtual int getCategoryFlags() const override { return EventCategoryInput | EventCategoryMouse; }
	};
}