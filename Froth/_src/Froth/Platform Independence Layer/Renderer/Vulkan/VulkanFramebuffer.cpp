#include <frothpch.h>
#include "VulkanFramebuffer.h"

namespace Froth
{
	VulkanFramebuffer::VulkanFramebuffer(const VulkanDevice& device, const VulkanImageView& imageView, U32 width, U32 height, const VulkanRenderPass& renderPass)
	{
		m_Device = &device;

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.pNext = nullptr;
		framebufferInfo.flags = 0;
		framebufferInfo.renderPass = renderPass.getRenderPass();
		framebufferInfo.attachmentCount = 1; // TODO: Parameterize?
		framebufferInfo.pAttachments = &imageView.getImageView();
		framebufferInfo.width = width;
		framebufferInfo.height = height;
		framebufferInfo.layers = 1; // TODO: Parameterize?

		// TODO: Assert
		if (vkCreateFramebuffer(m_Device->getDevice(), &framebufferInfo, nullptr, &m_Framebuffer) != VK_SUCCESS) std::cout << "Failed to create framebuffer!" << std::endl;	
	}

	VulkanFramebuffer::VulkanFramebuffer(VulkanFramebuffer&& other)
	{
		m_Device = other.m_Device;
		m_Framebuffer = other.m_Framebuffer;

		other.m_Framebuffer = nullptr;
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		if (m_Framebuffer != nullptr)
		{
			std::cout << "Framebuffer destroyed" << std::endl;
			vkDestroyFramebuffer(m_Device->getDevice(), m_Framebuffer, nullptr);
		}
	}

	VulkanFramebuffer& VulkanFramebuffer::operator=(VulkanFramebuffer&& other)
	{
		m_Device = other.m_Device;
		m_Framebuffer = other.m_Framebuffer;

		other.m_Framebuffer = nullptr;
		return *this;
	}

}