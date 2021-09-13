#pragma once
#include "Froth/Platform Independence Layer/PrimitiveTypes.h"

namespace Froth
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowMove, WindowFocus, WindowLoseFocus,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMove, MouseScroll
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BITFIELD(0),
		EventCategoryInput = BITFIELD(1),
		EventCategoryKeyboard = BITFIELD(2),
		EventCategoryMouse = BITFIELD(3),
		EventCategoryMouseButton = BITFIELD(4)
	};

	class Event
	{
	public:
		bool isHandled = false;

		virtual ~Event() = default;

		virtual EventType getEventType() const = 0;
		virtual int getCategoryFlags() const = 0;
		
		inline bool isInCategory(EventCategory category)
		{
			return getCategoryFlags() & category;
		}
	};
}