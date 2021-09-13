#pragma once
#include "Event.h"
#include <functional>

namespace Froth
{
	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	private:
		Event& m_Event;

	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		template<typename T>
		bool dispatch(EventFn<T> func)
		{
			if (m_Event.getEventType() == T::getStaticEventType())
			{
				m_Event.isHandled = func(*(T*) &m_Event);
				return true;
			}
			return false;
		}

	};
}