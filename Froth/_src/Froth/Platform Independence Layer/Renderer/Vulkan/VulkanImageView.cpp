#include <frothpch.h>
#include "VulkanImageView.h"

namespace Froth
{

	VulkanImageView::VulkanImageView(const VulkanDevice& device, const VkImage& image, const VkImageViewType& viewType, const VkFormat& format, U32 baseMipLevel, U32 levelCount, U32 baseArrayLayer, U32 layerCount)
	{
		m_Device = &device;

		VkImageViewCreateInfo imageViewInfo;
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.pNext = nullptr;
		imageViewInfo.flags = 0;
		imageViewInfo.image = image;
		imageViewInfo.viewType = viewType;
		imageViewInfo.format = format;

		// TODO: Parameterize
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY; // Can be changed to create monochrome textures or keep channels at constants
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewInfo.subresourceRange.baseMipLevel = baseMipLevel;
		imageViewInfo.subresourceRange.levelCount = levelCount;
		imageViewInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
		imageViewInfo.subresourceRange.layerCount = layerCount;

		// TODO: Assert
		if (vkCreateImageView(m_Device->getDevice(), &imageViewInfo, nullptr, &m_ImageView) != VK_SUCCESS) std::cout << "Failed to create image view!" << std::endl;
	}

	VulkanImageView::VulkanImageView(VulkanImageView&& other)
	{
		m_Device = other.m_Device;
		m_ImageView = other.m_ImageView;
		other.m_ImageView = nullptr;
	}

	VulkanImageView::~VulkanImageView()
	{
		destroyImageView();
	}

	void VulkanImageView::destroyImageView()
	{
		if (m_ImageView != nullptr)
		{
			std::cout << "ImageView destroyed" << std::endl;
			vkDestroyImageView(m_Device->getDevice(), m_ImageView, nullptr);
			m_ImageView = nullptr;
		}
	}

	VulkanImageView& VulkanImageView::operator=(VulkanImageView&& other)
	{
		m_Device = other.m_Device;
		m_ImageView = other.m_ImageView;
		other.m_ImageView = nullptr;
		return *this;
	}

}