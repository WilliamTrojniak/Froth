#include <frothpch.h>
#include "Froth/Platform Independence Layer/Window/WindowAPI.h"
#include "VulkanSurface.h"
#include "VulkanWinSurface.h"
#include "VulkanInstance.h"

namespace Froth
{

	VulkanSurface::VulkanSurface(VulkanSurface&& other)
	{
		m_Surface = other.m_Surface;
		other.m_Surface = nullptr;
	}

	VulkanSurface::~VulkanSurface()
	{
		if (m_Surface != nullptr)
		{
			std::cout << "Surface destroyed" << std::endl;
			vkDestroySurfaceKHR(VulkanInstance::get().getInstance(), m_Surface, nullptr);
		}
	}

	VulkanSurface& VulkanSurface::operator=(VulkanSurface&& other)
	{
		m_Surface = other.m_Surface;
		other.m_Surface = nullptr;
		return *this;
	}

	VulkanSurface VulkanSurface::createSurface(Window* pWindow)
	{

		switch (WindowAPI::getAPI())
		{
		case WindowAPI::API::WIN:
			return VulkanWinSurface(pWindow);
			
		default:
			break;
		}

	}


}