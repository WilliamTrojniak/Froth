#include <frothpch.h>
#include "VulkanWinSurface.h"
#include "VulkanInstance.h"

namespace Froth
{

	VulkanWinSurface::VulkanWinSurface(Window* pWindow)
	{
		VkWin32SurfaceCreateInfoKHR surfaceInfo{};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.pNext = nullptr;
		surfaceInfo.flags = 0;
		surfaceInfo.hinstance = *(HINSTANCE*) pWindow->getAppID(); // HACK: Find better solution
		surfaceInfo.hwnd = *(HWND*) pWindow->getWindow();

		// TODO: Assert
		if (vkCreateWin32SurfaceKHR(VulkanInstance::get().getInstance(), &surfaceInfo, nullptr, &m_Surface) != VK_SUCCESS) std::cout << "Failed to create surface" << std::endl;
	}

}