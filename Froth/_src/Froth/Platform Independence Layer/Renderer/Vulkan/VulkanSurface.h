#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Froth/Platform Independence Layer/Window/Window.h"
#include "Froth/Platform Independence Layer/Renderer/Vulkan/VulkanInstance.h"

namespace Froth
{
	class VulkanSurface
	{
	protected:
		VkSurfaceKHR m_Surface;
	
	public:
		VulkanSurface() = default;
		VulkanSurface(VulkanSurface& other) = delete;
		VulkanSurface(VulkanSurface&& other);
		~VulkanSurface();

		VulkanSurface& operator=(VulkanSurface&& other);

		static VulkanSurface createSurface(Window* pWindow);
		const VkSurfaceKHR& getSurface() const { return m_Surface; }
	
	protected:
		
	};

}