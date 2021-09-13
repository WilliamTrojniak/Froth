#pragma once
#include <string>
#include "Froth/Platform Independence Layer/PointerTypes.h"
#include <Froth/Core Layer/Events/Event.h>
#include <functional>
#include <thread>

namespace Froth
{
	class Window
	{
	protected:

		std::thread m_Thread;

		U32 m_Width;
		U32 m_Height;

		std::wstring m_Title;

		std::function<void(Event&)> m_EventCallbackFunction;

	public:

		virtual void onUpdate();

		virtual void pollEvents() = 0;

		// TODO: Find a better solution than void pointers
		virtual void* getWindow() = 0;
		virtual void* getAppID() = 0;

		void setEventCallbackFunction(std::function<void(Event&)>);

		
		/// <summary>
		/// </summary>
		/// <param name="width">Initial width of the window</param>
		/// <param name="height">Initial height of the window</param>
		/// <param name="title">Title of the window</param>
		/// <returns>
		///	Window* that must be manually deleted
		/// </returns>
		static Window* createWindow(U32 width, U32 height, const std::wstring& title);
	protected:
		Window(U32 width, U32 height, const std::wstring& title);
		void onEvent(Event& e);
	private:

	};
}
