#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanDevice.h"
#include "VulkanRenderPass.h"
#include "VulkanImageView.h"

namespace Froth
{
	class VulkanFramebuffer
	{
	public:
		VulkanFramebuffer() = default;
		VulkanFramebuffer(const VulkanDevice& device, const VulkanImageView& imageView, U32 width, U32 height, const VulkanRenderPass& renderPass);
		VulkanFramebuffer(VulkanFramebuffer& other) = delete;
		VulkanFramebuffer(VulkanFramebuffer&& other);
		~VulkanFramebuffer();

		const VkFramebuffer& getFramebuffer() const { return m_Framebuffer; }

		VulkanFramebuffer& operator=(VulkanFramebuffer&& other);

	private:
		const VulkanDevice* m_Device; 
		VkFramebuffer m_Framebuffer;
	};
}
