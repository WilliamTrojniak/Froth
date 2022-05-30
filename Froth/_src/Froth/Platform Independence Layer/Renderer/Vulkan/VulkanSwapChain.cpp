#include <frothpch.h>
#include "VulkanSwapChain.h"

namespace Froth
{

	VulkanSwapChain::VulkanSwapChain(const VulkanDevice& device, const VulkanSurface& surface)
	{
		m_Device = &device;
		m_Extent = m_Device->getSurfaceCapabilities(surface).currentExtent;
		
		m_Format = chooseSurfaceFormat(m_Device->getSurfaceFormats(surface));


		if (!(m_Device->getSurfaceCapabilities(surface).supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT))
		{
			// TODO: Assert
			std::cout << "The surface-device combination is not capable of being used in a frame buffer!" << std::endl;
		}

		createSwapChain(surface);		

		m_Images = retrieveImages();
		m_ImageViews = createImagesViews();
		
	}

	VulkanSwapChain::VulkanSwapChain(VulkanSwapChain&& other)
	{
		m_Device = other.m_Device;
		m_SwapChain = other.m_SwapChain;
		m_Format = other.m_Format;
		m_Extent = other.m_Extent;

		m_Images = std::move(other.m_Images);
		m_ImageViews = std::move(other.m_ImageViews);

		other.m_SwapChain = nullptr;
	}

	VulkanSwapChain::~VulkanSwapChain()
	{
		destroySwapChain();
	}

	void VulkanSwapChain::destroySwapChain()
	{
		if (m_SwapChain != nullptr)
		{
			destroyImageViews();
			std::cout << "Swapchain destroyed" << std::endl;
			vkDestroySwapchainKHR(m_Device->getDevice(), m_SwapChain, nullptr);
		}
	}

	void VulkanSwapChain::destroyImageViews()
	{
		for (size_t i = 0; i < m_ImageViews.size(); i++)
		{
			m_ImageViews[i].destroyImageView();
		}
	}

	VulkanSwapChain& VulkanSwapChain::operator=(VulkanSwapChain&& other)
	{
		m_Device = other.m_Device;
		m_SwapChain = other.m_SwapChain;
		m_Format = other.m_Format;
		m_Extent = other.m_Extent;

		m_Images = std::move(other.m_Images);
		m_ImageViews = std::move(other.m_ImageViews);

		other.m_SwapChain = nullptr;
		return *this;
	}

	U32 VulkanSwapChain::getImageCount() const
	{
		U32 swapChainImageCount = 0;
		vkGetSwapchainImagesKHR(m_Device->getDevice(), m_SwapChain, &swapChainImageCount, nullptr);
		return swapChainImageCount;
	}

	VkSurfaceFormatKHR VulkanSwapChain::chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) const
	{
		// TODO: Parameterize which format is chosen (Useful for enabling HDR, etc)

		U32 formatIndex, secondaryFormatIndex, tertiaryFormatIndex, fallbackFormatIndex;
		formatIndex = secondaryFormatIndex = tertiaryFormatIndex = fallbackFormatIndex = -1;



		for (U32 i = 0; i < formats.size(); i++)
		{
			if (formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) // First choice
			{
				formatIndex = i;
				//break;
			}
			if (formats[i].format == VK_FORMAT_B8G8R8A8_UNORM && formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) // Second choice
			{
				secondaryFormatIndex = i;
			}
			else if (formats[i].format == VK_FORMAT_A2B10G10R10_UNORM_PACK32 && formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) // HDR?
			{
				tertiaryFormatIndex = i;
			}
			fallbackFormatIndex = i;
		}
		if (fallbackFormatIndex == -1)
		{
			// TODO: Assert
			std::cout << "Failed to find choose a surface format!" << std::endl;
		}

		else if (formatIndex == -1)
		{
			if (secondaryFormatIndex != -1)
				formatIndex = secondaryFormatIndex;
			else if (tertiaryFormatIndex != -1)
				formatIndex = tertiaryFormatIndex;
			else
				formatIndex = fallbackFormatIndex;
		}

		return formats[formatIndex];
	}

	void VulkanSwapChain::createSwapChain(const VulkanSurface& surface, const VkSwapchainKHR& oldSwapchain)
	{
		VkSwapchainCreateInfoKHR swapChainInfo{};
		swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainInfo.pNext = nullptr;
		swapChainInfo.flags = 0;
		swapChainInfo.surface = surface.getSurface();
		swapChainInfo.minImageCount = max(m_Device->getSurfaceCapabilities(surface).minImageCount, 2);
		swapChainInfo.imageFormat = m_Format.format;
		swapChainInfo.imageColorSpace = m_Format.colorSpace;
		swapChainInfo.imageExtent = getExtent();
		swapChainInfo.imageArrayLayers = 1; // TODO: Parameterize?
		swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainInfo.queueFamilyIndexCount = 0; // Ignored with VK_SHARING_MODE_EXCLUSIVE
		swapChainInfo.pQueueFamilyIndices = nullptr; // Ignored with VK_SHARING_MODE_EXCLUSIVE
		swapChainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR; // TODO: Rotate images to accommodate portrait displays
		swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // Windows are opaque
		swapChainInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR; // V-Sync // TODO: Parameterize
		swapChainInfo.clipped = VK_TRUE; // Vulkan does not render parts of the window that are clipped (by other windows, etc.)
		swapChainInfo.oldSwapchain = oldSwapchain; // Change if remaking swap chain
		// TODO: Assert
		if (vkCreateSwapchainKHR(m_Device->getDevice(), &swapChainInfo, nullptr, &m_SwapChain) != VK_SUCCESS) std::cout << "Failed to create swap chain" << std::endl;
	}

	void VulkanSwapChain::recreateSwapChain(const VulkanSurface& surface)
	{
		m_Extent = m_Device->getSurfaceCapabilities(surface).currentExtent;
		m_Format = chooseSurfaceFormat(m_Device->getSurfaceFormats(surface));

		if (!(m_Device->getSurfaceCapabilities(surface).supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT))
		{
			// TODO: Assert
			std::cout << "The surface-device combination is not capable of being used in a frame buffer!" << std::endl;
		}

		
		createSwapChain(surface, m_SwapChain);

		m_Images = retrieveImages();
		m_ImageViews = createImagesViews(); // TODO: Make sure that the destructor is not destroying image views that are being carried on
	}

	std::vector<VkImage> VulkanSwapChain::retrieveImages()
	{
		U32 swapChainImageCount = 0;
		vkGetSwapchainImagesKHR(m_Device->getDevice(), m_SwapChain, &swapChainImageCount, nullptr);
		
		std::vector<VkImage> images(swapChainImageCount);
		vkGetSwapchainImagesKHR(m_Device->getDevice(), m_SwapChain, &swapChainImageCount, images.data());

		return images;
		
	}

	std::vector<VulkanImageView> VulkanSwapChain::createImagesViews()
	{
		std::vector<VulkanImageView> imageViews;
		for (U32 i = 0; i < m_Images.size(); i++)
		{
			imageViews.push_back(VulkanImageView(*m_Device, m_Images[i], VK_IMAGE_VIEW_TYPE_2D, m_Format.format));
		}
		return imageViews;
	}

}