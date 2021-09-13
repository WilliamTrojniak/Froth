#pragma once
#include "WindowsBaseWindow.h"
#include <thread>

namespace Froth
{
	
	class WindowsWindow : public BaseWindowsWindow
	{
	friend class Window;

	private:
		static bool s_ClassIsRegistered;


	protected:
		WindowsWindow(U32 width, U32 height, const std::wstring& title);	
			
	public:
		~WindowsWindow();

		PCWSTR getClassName() const override { return L"MainClass"; }
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
		
	};
	

}