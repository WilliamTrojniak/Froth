#include "frothpch.h"
#include "WindowsBaseWindow.h"

namespace Froth
{
	std::map<HWND, BaseWindowsWindow*> BaseWindowsWindow::s_WindowMap;

	BaseWindowsWindow::~BaseWindowsWindow()
	{
		s_WindowMap.erase(m_HWnd);
	}

	bool BaseWindowsWindow::registerClass()
	{
		WNDCLASS wc = { 0 };

		wc.style = CS_GLOBALCLASS | CS_DBLCLKS;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = m_HInstance;
		wc.hIcon = NULL; // TODO: Provide a means of setting the icon
		wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(32512));
		wc.hbrBackground = NULL; // Window must paint its own background whenever it is requested to paint its client area.
		wc.lpszMenuName = NULL; // Windows belonging to this class have no default menu
		wc.lpszClassName = getClassName(); // Must be less than 256 characters in length

		bool result = (RegisterClass(&wc));
		// TODO: Assert if failed to register class 

		return result;
	}

	bool BaseWindowsWindow::createWindow()
	{
		m_HWnd = CreateWindowEx(0, getClassName(), m_Title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_Width, m_Height, 0, 0, m_HInstance, this);
		bool result = (m_HWnd != NULL);

		if (result)
		{
			s_WindowMap[m_HWnd] = this;
			ShowWindow(m_HWnd, SW_SHOWNORMAL);
		}

		//TODO: Assert if failed to create window
		return result;
	}

	void BaseWindowsWindow::pollEvents()
	{
		MSG msg = {};

		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // HACK: This creates a busy loop, would be better to use getMessage() on seperate thread
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	LRESULT CALLBACK BaseWindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg != WM_NCCREATE)
		{
			auto it = s_WindowMap.find(hwnd);
			if (it != s_WindowMap.end())
			{
				return it->second->HandleMessage(uMsg, wParam, lParam);
			}
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

}