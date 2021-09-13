#include <frothpch.h>
#include "VulkanCommandBuffer.h"

namespace Froth
{

	VulkanCommandBuffer::VulkanCommandBuffer(const VulkanDevice& device, const VulkanCommandPool& commandPool)
		: m_Device(&device), m_Pool(&commandPool)
	{
		VkCommandBufferAllocateInfo commandBufferAllocateInfo;
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.pNext = nullptr;
		commandBufferAllocateInfo.commandPool = m_Pool->getCommandPool();
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocateInfo.commandBufferCount = 1; // TODO: Parameterize for mass creation of command buffers

		// TODO: Assert
		if (vkAllocateCommandBuffers(m_Device->getDevice(), &commandBufferAllocateInfo, &m_CommandBuffer) != VK_SUCCESS) std::cout << "Failed to allocate command buffer!" << std::endl;
	}

	VulkanCommandBuffer::VulkanCommandBuffer(VulkanCommandBuffer&& other)
		: m_Device(other.m_Device), m_Pool(other.m_Pool), m_CommandBuffer(other.m_CommandBuffer)
	{
		other.m_CommandBuffer = nullptr;
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		if (m_CommandBuffer != nullptr)
		{
			std::cout << "CommandBuffer destroyed" << std::endl;
			vkFreeCommandBuffers(m_Device->getDevice(), m_Pool->getCommandPool(), 1, &m_CommandBuffer);
		}
	}

	void VulkanCommandBuffer::begin() const
	{
		VkCommandBufferBeginInfo commandBufferBeginInfo;
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.pNext = nullptr;
		commandBufferBeginInfo.flags = 0;
		commandBufferBeginInfo.pInheritanceInfo = nullptr; // Used for secondary command buffers

		vkBeginCommandBuffer(m_CommandBuffer, &commandBufferBeginInfo);
	}

	void VulkanCommandBuffer::end() const
	{
		// TODO: Assert
		if (vkEndCommandBuffer(m_CommandBuffer) != VK_SUCCESS) std::cout << "Failed to record to command buffer!" << std::endl;
	}

	void VulkanCommandBuffer::beginRenderPass(const VulkanRenderPass& renderPass, const VulkanFramebuffer& frameBuffer, const VkRect2D& renderArea) const
	{
		const VkClearValue CLEAR_COLOR = { 0.0f, 0.0f, 0.0f, 1.0f }; // Can be extended to clear depth and stencil values too
		VkRenderPassBeginInfo renderPassBeginInfo {
			VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			nullptr,
			renderPass.getRenderPass(),
			frameBuffer.getFramebuffer(),
			renderArea,
			1,
			&CLEAR_COLOR
		};

		vkCmdBeginRenderPass(m_CommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE); // Third parameter indicates whether the command will be called by a primary or secondary command buffer
	}

	void VulkanCommandBuffer::endRenderPass() const
	{
		vkCmdEndRenderPass(m_CommandBuffer);
	}


	void VulkanCommandBuffer::bindGraphicsPipeline(const VulkanGraphicsPipeline& pipeline) const
	{
		vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipeline());
	}

	void VulkanCommandBuffer::bindVertexBuffer(const VulkanVertexBuffer& vertexBuffer) const
	{
		const VkDeviceSize OFFSETS[] = { 0 };
		vkCmdBindVertexBuffers(m_CommandBuffer, 0, 1, &vertexBuffer.getBuffer(), OFFSETS);
	}

	void VulkanCommandBuffer::bindIndexBuffer(const VulkanIndexBuffer& indexBuffer) const
	{
		vkCmdBindIndexBuffer(m_CommandBuffer, indexBuffer.getBuffer(), 0, VK_INDEX_TYPE_UINT16);
	}

	void VulkanCommandBuffer::bindGraphicsDescriptorSets(const VulkanPipelineLayout& pipelineLayout, U32 descriptorSetCount, VulkanDescriptorSet* descriptorSets) const
	{
		std::vector<VkDescriptorSet> rawDescSets(descriptorSetCount);
		for (U32 i = 0; i < descriptorSetCount; i++)
		{
			rawDescSets[i] = descriptorSets[i].getSet();
		}
		vkCmdBindDescriptorSets(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout.getLayout(), 0, descriptorSetCount, rawDescSets.data(), 0, nullptr);
	}

	void VulkanCommandBuffer::drawIndexed(U32 indexCount) const
	{
		vkCmdDrawIndexed(m_CommandBuffer, indexCount, 1, 0, 0, 0);
	}

	VulkanCommandBuffer& VulkanCommandBuffer::operator=(VulkanCommandBuffer&& other)
	{
		m_Device = other.m_Device;
		m_Pool = other.m_Pool;
		m_CommandBuffer = other.m_CommandBuffer;

		other.m_CommandBuffer = nullptr;
		return *this;
	}

}