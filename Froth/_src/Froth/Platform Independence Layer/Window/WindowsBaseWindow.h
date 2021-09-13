#pragma once
#include <windows.h>
#include "Window.h"
#include <codecvt>
#include <thread>
#include <map>

namespace Froth
{
	//template <class DERIVED_TYPE>
	class BaseWindowsWindow : public Window
	{
	private:
		static std::map<HWND, BaseWindowsWindow*> s_WindowMap;

	protected:

		BaseWindowsWindow(U32 width, U32 height, const std::wstring& title)
			: Window::Window(width, height, title), m_HInstance(GetModuleHandle(NULL)), m_HWnd(NULL)
		{
		}

	public:
		~BaseWindowsWindow();

		void* getWindow() override { return &m_HWnd; }

		void* getAppID() override { return &m_HInstance; }

		bool registerClass();

		bool createWindow();

		void pollEvents() override;

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:

		virtual PCWSTR getClassName() const = 0;
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

		HINSTANCE m_HInstance;
		HWND m_HWnd;
	};

	
}