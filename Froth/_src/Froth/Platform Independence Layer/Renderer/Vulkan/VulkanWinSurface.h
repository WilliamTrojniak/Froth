#pragma once
#include "VulkanSurface.h"

namespace Froth
{
	class VulkanWinSurface : public VulkanSurface
	{
	public:
		VulkanWinSurface(Window* pWindow);
	};
}