#pragma once
#include "Froth/Core Layer/Events/Event.h"
#include "Froth/Core Layer/Events/ApplicationEvent.h"
#include "Froth/Core Layer/Events/KeyEvent.h"
#include "Froth/Core Layer/Events/MouseEvent.h"

namespace Froth
{
	class Layer
	{
	protected:
		Layer() = default;
	public:
		virtual ~Layer() {}

		virtual void onAttach() {}
		virtual void onDetatch() {}
		virtual void onUpdate() {}

		virtual bool onWindowClose(WindowCloseEvent& e) { return false; }
		virtual bool onWindowResize(WindowResizeEvent& e) { return false; }
		virtual bool onWindowMove(WindowMoveEvent& e) { return false; }
		virtual bool onWindowFocus(WindowFocusEvent& e) { return false; }
		virtual bool onWindowLoseFocus(WindowLoseFocusEvent& e) { return false; }
		virtual bool onKeyPressed(KeyPressedEvent& e) { return false; }
		virtual bool onKeyReleased(KeyReleasedEvent& e) { return false; }
		virtual bool onMouseButtonPressed(MouseButtonPressedEvent& e) { return false; }
		virtual bool onMouseButtonReleased(MouseButtonReleasedEvent& e) { return false; }
		virtual bool onMouseMove(MouseMoveEvent& e) { return false; }
		virtual bool onMouseScroll(MouseScrollEvent& e) { return false; }
	};
}