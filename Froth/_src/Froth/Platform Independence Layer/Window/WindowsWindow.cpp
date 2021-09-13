#include "frothpch.h"
#include "WindowsWindow.h"
#include "Froth/Core Layer/Events/ApplicationEvent.h"


//TODO: Clean up the interface

namespace Froth
{
	bool WindowsWindow::s_ClassIsRegistered = false;

	

	WindowsWindow::WindowsWindow(U32 width, U32 height, const std::wstring& title)
		: BaseWindowsWindow(width, height, title)
	{
		// Have to call after BaseWindow is constructed since depends on getClassName() being overriden
		if (!s_ClassIsRegistered)
		{
			if (registerClass())
				s_ClassIsRegistered = true;
			else
				// TODO: Better debug message
				std::cout << "Failed to register WindowsWindow window class" << std::endl;
		}

		// TODO: Better debug message
		if (!createWindow()) std::cout << "Failed to create WindowsWindow window" << std::endl;

	}


	WindowsWindow::~WindowsWindow()
	{
		m_Thread.join();
	}

	LRESULT WindowsWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		{

			switch (uMsg)
			{
			case WM_DESTROY:
				onEvent(WindowCloseEvent());
				PostQuitMessage(0);
				return 0;

			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(m_HWnd, &ps);
				FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));

				EndPaint(m_HWnd, &ps);
				return 0;
			}

			/*
			Mouse button flags and macros:
			DragDetect(HWND, {mousex, mousey})
			ClipCursor(RECT) Confines mouse to rectangle in screen coordinates

			LParam:
			GET_X_LPARAM, GET_Y_LPARAM

			WParam:
			MK_CONTROL	The CTRL key is down.
			MK_LBUTTON	The left mouse button is down.
			MK_MBUTTON	The middle mouse button is down.
			MK_RBUTTON	The right mouse button is down.
			MK_SHIFT	The SHIFT key is down.
			MK_XBUTTON1	The XBUTTON1 button is down.
			MK_XBUTTON2	The XBUTTON2 button is down.
			*/

			case WM_LBUTTONDOWN: // Secondary mouse click
				return 0;
			case WM_LBUTTONUP: // Secondary mouse button release
				return 0;
			case WM_LBUTTONDBLCLK: // Secondary mouse button double click
				return 0;
			case WM_MBUTTONDOWN: // Middle mouse button down
				return 0;
			case WM_MBUTTONUP: // Middle mouse button up
				return 0;
			case WM_MBUTTONDBLCLK: //Middle mouse button double click
				return 0;
			case WM_RBUTTONDOWN: // Primary mouse button down
				return 0;
			case WM_RBUTTONUP: // Primary mouse button release
				return 0;
			case WM_RBUTTONDBLCLK: // Primary mouse button double click
				return 0;
			case WM_XBUTTONDOWN: // Side button 1 or 2 down
				return 0;
			case WM_XBUTTONUP: // Side button 1 or 2 release
				return 0;
			case WM_XBUTTONDBLCLK: // Side button 1 or 2 double click
				return 0;
			case WM_MOUSEWHEEL: // Mouse wheel scroll (+: away, -: toward user)
				return 0;
			case WM_MOUSEMOVE: // Mouse movement within client area
				//hCursor = LoadCursor(NULL, MAKEINTRESOURCE(32512));
				//SetCursor(hCursor);
				//return 0;

			case WM_SYSKEYDOWN: // ALT + key press
				return 0;
			case WM_SYSKEYUP: // ALT + key release
				return 0;
			case WM_KEYDOWN: // Keyboard key press
				return 0;
			case WM_KEYUP: // Keyboard key release
				return 0;

			}
			return DefWindowProc(m_HWnd, uMsg, wParam, lParam);

		}
	}

}