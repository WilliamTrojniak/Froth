#include <frothpch.h>
#include "VulkanRenderPass.h"

namespace Froth
{

	VulkanRenderPass::VulkanRenderPass(const VulkanDevice& device, U32 attachmentCount, VkAttachmentDescription* attachments, U32 subpassCount, VkSubpassDescription* subpasses, U32 dependencyCount, VkSubpassDependency* dependencies)
	{
		m_Device = &device;

		VkRenderPassCreateInfo renderPassInfo;
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.pNext = nullptr;
		renderPassInfo.flags = 0;
		renderPassInfo.attachmentCount = attachmentCount;
		renderPassInfo.pAttachments = attachments;
		renderPassInfo.subpassCount = subpassCount;
		renderPassInfo.pSubpasses = subpasses;
		renderPassInfo.dependencyCount = dependencyCount;
		renderPassInfo.pDependencies = dependencies;

		// TODO: Assert
		if (vkCreateRenderPass(m_Device->getDevice(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS) std::cout << "Failed to create render pass!" << std::endl; 		
	}

	VulkanRenderPass::VulkanRenderPass(VulkanRenderPass&& other)
	{
		m_Device = other.m_Device;
		m_RenderPass = other.m_RenderPass;
		other.m_RenderPass = nullptr;
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
		if (m_RenderPass != nullptr)
		{
			std::cout << "RenderPass destroyed" << std::endl;
			vkDestroyRenderPass(m_Device->getDevice(), m_RenderPass, nullptr);
		}
	}

	VulkanRenderPass& VulkanRenderPass::operator=(VulkanRenderPass&& other)
	{
		m_Device = other.m_Device;
		m_RenderPass = other.m_RenderPass;
		other.m_RenderPass = nullptr;

		return *this;
	}

	VkAttachmentDescription VulkanRenderPass::createAttachmentDesc(const VkFormat& format, const VkSampleCountFlagBits& samples, const VkAttachmentLoadOp& loadOp, const VkAttachmentStoreOp& storeOp, const VkAttachmentLoadOp& stencilLoadOp, const VkAttachmentStoreOp& stencilStoreOp, const VkImageLayout& initialLayout, const VkImageLayout& finalLayout)
	{
		return VkAttachmentDescription {
			0, //flags
			format,
			samples,
			loadOp,
			storeOp,
			stencilLoadOp,
			stencilStoreOp,
			initialLayout,
			finalLayout
		};
	}

	VkAttachmentReference VulkanRenderPass::createAttachmentRef(U32 attachmentIndex, const VkImageLayout& layout)
	{
		return VkAttachmentReference {
			attachmentIndex, // Attachment
			layout // Layout
		};
	}

	VkSubpassDescription VulkanRenderPass::createSubpass(U32 inputAttachmentCount, VkAttachmentReference* inputAttachments, U32 colorAttachmentCount, VkAttachmentReference* colorAttachments, VkAttachmentReference* resolveAttachments, VkAttachmentReference* depthStencilAttachments, U32 preserveAttachmentCount, U32* preserveAttachments, const VkPipelineBindPoint& pipelineBindPoint)
	{
		return VkSubpassDescription {
			0, // flags
			pipelineBindPoint,
			inputAttachmentCount, 
			inputAttachments, 
			colorAttachmentCount, 
			colorAttachments,
			resolveAttachments, // For multi-sampling
			depthStencilAttachments, // Only ever 1 thus not array
			preserveAttachmentCount,
			preserveAttachments
		};
	}

	VkSubpassDependency VulkanRenderPass::createDependency(U32 srcSubpass, U32 dstSubpass, const VkPipelineStageFlags& srcStageMask, const VkPipelineStageFlags& dstStageMask, const VkAccessFlags& srcAccessMask, const VkAccessFlags& dstAccessMask, const VkDependencyFlags& dependencyFlags)
	{
		return VkSubpassDependency {
			srcSubpass,
			dstSubpass,
			srcStageMask,
			dstStageMask,
			srcAccessMask,
			dstAccessMask,
			dependencyFlags
		};
	}

}