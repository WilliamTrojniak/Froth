#pragma once
#include "Base.h"

// Temp
#include "Froth/Platform Independence Layer/Renderer/Vulkan/VulkanRenderer.h"


namespace Froth
{
	class Application
	{
	private:
		bool m_Running = true;
		Window* m_Window;
		LayerStack m_LayerStack;

		// Temp
		VulkanRenderer m_Renderer;

	public:
		Application();
		~Application();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		void Run();

		void onEvent(Event& e);
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);
		bool onWindowMove(WindowMoveEvent& e);
		bool onWindowFocus(WindowFocusEvent& e);
		bool onWindowLoseFocus(WindowLoseFocusEvent& e);
		bool onKeyPressed(KeyPressedEvent& e);
		bool onKeyReleased(KeyReleasedEvent& e);
		bool onMouseButtonPressed(MouseButtonPressedEvent& e);
		bool onMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool onMouseMove(MouseMoveEvent& e);
		bool onMouseScroll(MouseScrollEvent& e);

	};

	Application* CreateApplication();
}