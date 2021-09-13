#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanSurface.h"
#include "VulkanSwapChain.h"
#include "VulkanCommandPool.h"
#include "VulkanImage.h"
#include "VulkanRenderPass.h"
#include "VulkanFramebuffer.h"
#include "VulkanDescriptorPool.h"
#include "VulkanUniformBuffer.h"
#include "VulkanDescriptorSetLayout.h"
#include "VulkanDescriptorSet.h"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanShaderModule.h"
#include "VulkanPipelineLayout.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanCommandBuffer.h"
#include "VulkanFence.h"
#include "VulkanSemaphore.h"

namespace Froth
{
	class VulkanRenderer
	{
	private:
		
		// IMPORTANT: ORDER MATTERS
		VulkanSurface m_Surface; // Depends on VulkanInstance
		VulkanDevice m_Device; // Depends on Surface
		VulkanSwapChain m_SwapChain; // Depends on Device & Surface
		VulkanCommandPool m_PresentationCommandPool;
		VulkanCommandPool m_GraphicsCommandPool;
		VulkanCommandPool m_ComputeCommandPool;
		VulkanCommandPool m_TransferCommandPool;
		VulkanRenderPass m_RenderPass;
		std::vector<VulkanFramebuffer> m_Framebuffers;
		std::vector<VulkanUniformBuffer> m_UniformBuffers;
		VulkanDescriptorPool m_DescriptorPool;
		VulkanDescriptorSetLayout m_DescriptorSetLayout;
		std::vector<VulkanDescriptorSet> m_DescriptorSets;
		VulkanVertexBuffer m_VertexBuffer;
		VulkanIndexBuffer m_IndexBuffer;
		VulkanShaderModule m_VertexShaderModule;
		VulkanShaderModule m_FragmentShaderModule;
		VulkanPipelineLayout m_PipelineLayout;
		VulkanGraphicsPipeline m_GraphicsPipeline;
		std::vector<VulkanCommandBuffer> m_GraphicsCommandBuffers;

		std::vector<VulkanSemaphore> m_PresentSemaphores;
		std::vector<VulkanSemaphore> m_CmdSubmitSemaphores;
		std::vector<VulkanFence> m_FrameFinishedFences;
		std::vector<VkFence> m_FrameInProgressFences;
		const U32 MAX_FRAMES_IN_FLIGHT = 2;
		U32 m_CurrentFrame = 0;

	public:
		VulkanRenderer(Window* pWindow); // TODO: Make protected
		~VulkanRenderer();

		void onUpdate();

	private:
		VkPhysicalDevice selectPhysicalDevice();
		bool checkDeviceExtensionsSupport(const VkPhysicalDevice& device, const std::vector<const char*>& extensions);
		
	};

}