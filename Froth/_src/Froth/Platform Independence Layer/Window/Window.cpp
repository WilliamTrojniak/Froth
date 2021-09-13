#include "frothpch.h"
#include "Froth/Platform Independence Layer/Window/Window.h"
#include "WindowAPI.h"
#include "WindowsWindow.h"

namespace Froth
{

	void Window::onUpdate()
	{
		pollEvents();
	}

	void Window::setEventCallbackFunction(std::function<void(Event&)> func)
	{
		m_EventCallbackFunction = func;
	}

	void Window::onEvent(Event& e)
	{
		if (m_EventCallbackFunction != nullptr)
		{
			m_EventCallbackFunction(e);
		}
	}

	Window* Window::createWindow(U32 width, U32 height, const std::wstring& title)
	{
		switch (WindowAPI::getAPI())
		{
		case WindowAPI::API::NONE:
			std::cout << "No suitable WindowAPI selected to create a window!" << std::endl;

		case WindowAPI::API::WIN:
			return new WindowsWindow(width, height, title);
		
		default:
			break;
		}
	}

	Window::Window(U32 width, U32 height, const std::wstring& title)
		: m_Width(width), m_Height(height), m_Title(title), m_EventCallbackFunction(nullptr)
	{

	}

}