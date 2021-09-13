#pragma once
#include "VulkanCommandPool.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanDescriptorSet.h"
#include "VulkanFramebuffer.h"

namespace Froth
{
	class VulkanCommandBuffer
	{
	public:
		VulkanCommandBuffer() = default;
		VulkanCommandBuffer(const VulkanDevice& device, const VulkanCommandPool& commandPool);
		VulkanCommandBuffer(VulkanCommandBuffer& other) = delete;
		VulkanCommandBuffer(VulkanCommandBuffer&& other);
		~VulkanCommandBuffer();

		VulkanCommandBuffer& operator=(VulkanCommandBuffer&& other);

		const VkCommandBuffer& getCommandBuffer() const { return m_CommandBuffer; }
		void begin() const;
		void end() const;
		void beginRenderPass(const VulkanRenderPass& renderPass, const VulkanFramebuffer& frameBuffer, const VkRect2D& renderArea) const;
		void endRenderPass() const;
		void bindGraphicsPipeline(const VulkanGraphicsPipeline& pipeline) const;
		void bindVertexBuffer(const VulkanVertexBuffer& vertexBuffer) const;
		void bindIndexBuffer(const VulkanIndexBuffer& indexBuffer) const;
		void bindGraphicsDescriptorSets(const VulkanPipelineLayout& pipelineLayout, U32 descriptorSetCount, VulkanDescriptorSet* descriptorSets) const;
		void drawIndexed(U32 indexCount) const;



	private:
		const VulkanDevice* m_Device;
		const VulkanCommandPool* m_Pool;
		VkCommandBuffer m_CommandBuffer;
	};
}