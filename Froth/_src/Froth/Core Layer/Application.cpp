#include "frothpch.h"
#include "Application.h"




namespace Froth
{
#define BIND_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)
	
	Application::Application()
		: m_Window(Window::createWindow(640, 480, L"Froth APP")), m_Renderer(m_Window)
	{
		// TODO: Ensure that only one application is only ever created
		m_Window->setEventCallbackFunction(BIND_FUNC(onEvent));

		
	}

	Application::~Application()
	{
		delete m_Window;
	}

	void Application::pushLayer(Layer* layer)
	{
		m_LayerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* overlay)
	{
		m_LayerStack.pushOverlay(overlay);
	}

	void Application::Run()
	{
		while (m_Running == true)
		{
			for (Layer* layer : m_LayerStack)
				layer->onUpdate();

			// Temp
			m_Renderer.onUpdate();
			m_Window->onUpdate();
		}
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_FUNC(onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(BIND_FUNC(onWindowResize));
		dispatcher.dispatch<WindowMoveEvent>(BIND_FUNC(onWindowMove));
		dispatcher.dispatch<WindowFocusEvent>(BIND_FUNC(onWindowFocus));
		dispatcher.dispatch<WindowLoseFocusEvent>(BIND_FUNC(onWindowLoseFocus));
		dispatcher.dispatch<KeyPressedEvent>(BIND_FUNC(onKeyPressed));
		dispatcher.dispatch<KeyReleasedEvent>(BIND_FUNC(onKeyReleased));
		dispatcher.dispatch<MouseButtonPressedEvent>(BIND_FUNC(onMouseButtonPressed));
		dispatcher.dispatch<MouseButtonReleasedEvent>(BIND_FUNC(onMouseButtonReleased));
		dispatcher.dispatch<MouseMoveEvent>(BIND_FUNC(onMouseMove));
		dispatcher.dispatch<MouseScrollEvent>(BIND_FUNC(onMouseScroll));
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->onWindowClose(e);
			if (e.isHandled)
				break;
		}

		m_Running = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->onWindowResize(e);
			if (e.isHandled)
				break;
		}

		return true;
	}

	bool Application::onWindowMove(WindowMoveEvent& e)
	{
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->onWindowMove(e);
			if (e.isHandled)
				break;
		}
		return true;
	}

	bool Application::onWindowFocus(WindowFocusEvent& e)
	{
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->onWindowFocus(e);
			if (e.isHandled)
				break;
		}
		return true;
	}

	bool Application::onWindowLoseFocus(WindowLoseFocusEvent& e)
	{
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->onWindowLoseFocus(e);
			if (e.isHandled)
				break;
		}
		return true;
	}

	bool Application::onKeyPressed(KeyPressedEvent& e)
	{
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->onKeyPressed(e);
			if (e.isHandled)
				break;
		}
		return true;
	}

	bool Application::onKeyReleased(KeyReleasedEvent& e)
	{
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->onKeyReleased(e);
			if (e.isHandled)
				break;
		}
		return true;
	}

	bool Application::onMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->onMouseButtonPressed(e);
			if (e.isHandled)
				break;
		}
		return true;
	}

	bool Application::onMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->onMouseButtonReleased(e);
			if (e.isHandled)
				break;
		}
		return true;
	}

	bool Application::onMouseMove(MouseMoveEvent& e)
	{
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->onMouseMove(e);
			if (e.isHandled)
				break;
		}
		return true;
	}

	bool Application::onMouseScroll(MouseScrollEvent& e)
	{
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->onMouseScroll(e);
			if (e.isHandled)
				break;
		}
		return true;
	}

}
