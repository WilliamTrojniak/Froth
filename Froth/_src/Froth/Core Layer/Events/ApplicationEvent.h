#pragma once
#include "Event.h"

namespace Froth
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		static EventType getStaticEventType() { return EventType::WindowClose; }

		virtual EventType getEventType() const override { return getStaticEventType(); }

		virtual int getCategoryFlags() const override { return EventCategoryApplication; }
	};

	class WindowResizeEvent : public Event
	{
	private:
		U32 m_Width, m_Height;

	public:
		WindowResizeEvent(U32 width, U32 height) : m_Width(width), m_Height(height) {}

		inline U32 getWidth() const { return m_Width; }
		inline U32 getHeight() const { return m_Height; }

		static EventType getStaticEventType() { return EventType::WindowResize; }

		virtual EventType getEventType() const override { return getStaticEventType(); }

		virtual int getCategoryFlags() const override { return EventCategoryApplication; }
	};

	class WindowMoveEvent : public Event
	{
	private:
		struct Position
		{
			U32 x, y;
		} m_Position;

	public:
		WindowMoveEvent(U32 x, U32 y) : m_Position({ x, y }) {}

		inline Position getWidth() const { return m_Position; }

		static EventType getStaticEventType() { return EventType::WindowMove; }

		virtual EventType getEventType() const override { return getStaticEventType(); }

		virtual int getCategoryFlags() const override { return EventCategoryApplication; }
	};

	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent() = default;

		static EventType getStaticEventType() { return EventType::WindowFocus; }

		virtual EventType getEventType() const override { return getStaticEventType(); }

		virtual int getCategoryFlags() const override { return EventCategoryApplication; }
	};

	class WindowLoseFocusEvent : public Event
	{
	public:
		WindowLoseFocusEvent() = default;

		static EventType getStaticEventType() { return EventType::WindowLoseFocus; }

		virtual EventType getEventType() const override { return getStaticEventType(); }

		virtual int getCategoryFlags() const override { return EventCategoryApplication; }
	};
}