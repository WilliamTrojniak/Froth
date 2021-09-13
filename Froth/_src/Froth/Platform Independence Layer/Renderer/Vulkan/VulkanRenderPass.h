#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanDevice.h"
//#include "VulkanFramebuffer.h"
//#include "VulkanCommandBuffer.h"

namespace Froth
{
	class VulkanRenderPass
	{
	public:
		VulkanRenderPass() = default;
		VulkanRenderPass(const VulkanDevice& device, U32 attachmentCount = 0, VkAttachmentDescription* attachments = nullptr, U32 subpassCount = 0, VkSubpassDescription* subpasses = nullptr, U32 dependencyCount = 0, VkSubpassDependency* dependencies = nullptr);
		VulkanRenderPass(VulkanRenderPass& other) = delete;
		VulkanRenderPass(VulkanRenderPass&& other);
		~VulkanRenderPass();

		VulkanRenderPass& operator=(VulkanRenderPass&& other);

		VkRenderPass getRenderPass() const { return m_RenderPass; }

		static VkAttachmentDescription createAttachmentDesc(const VkFormat& format, const VkSampleCountFlagBits& samples = VK_SAMPLE_COUNT_1_BIT, const VkAttachmentLoadOp& loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR, const VkAttachmentStoreOp& storeOp = VK_ATTACHMENT_STORE_OP_STORE, const VkAttachmentLoadOp& stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE, const VkAttachmentStoreOp& stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE, const VkImageLayout& initialLayout = VK_IMAGE_LAYOUT_UNDEFINED, const VkImageLayout& finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		static VkAttachmentReference createAttachmentRef(U32 attachmentIndex, const VkImageLayout& layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		static VkSubpassDescription createSubpass(U32 inputAttachmentCount = 0, VkAttachmentReference* inputAttachments = nullptr, U32 colorAttachmentCount = 0, VkAttachmentReference* colorAttachments = nullptr, VkAttachmentReference* resolveAttachments = nullptr, VkAttachmentReference* depthStencilAttachments = nullptr, U32 preserveAttachmentCount = 0, U32* preserveAttachments = nullptr, const VkPipelineBindPoint& pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);
		static VkSubpassDependency createDependency(U32 srcSubpass = VK_SUBPASS_EXTERNAL, U32 dstSubpass = 0, const VkPipelineStageFlags& srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, const VkPipelineStageFlags& dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, const VkAccessFlags& srcAccessMask = 0, const VkAccessFlags& dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, const VkDependencyFlags& dependencyFlags = 0);

	private:
		const VulkanDevice* m_Device; // HACK: Device not owned by render pass, used for cleanup
		VkRenderPass m_RenderPass;
	};
}