#include <frothpch.h>
#include "VulkanRenderer.h"
#include "FMath/FMath.h"

namespace Froth
{
	struct UniformBufferObject { 
		alignas(16) Froth::Math::Matrix4D model;
		alignas(16) Froth::Math::Matrix4D view;
		alignas(16) Froth::Math::Matrix4D projection;
	};
	
	VulkanRenderer::VulkanRenderer(Window* pWindow)
	{
		m_Surface = VulkanSurface::createSurface(pWindow);
		m_Device = VulkanDevice(selectPhysicalDevice(), m_Surface);
		m_PresentationCommandPool = VulkanCommandPool(m_Device, m_Device.getPresentQueueIndex());
		m_GraphicsCommandPool = VulkanCommandPool(m_Device, m_Device.getGraphicsQueueIndex());
		m_ComputeCommandPool = VulkanCommandPool(m_Device, m_Device.getComputeQueueIndex());
		m_TransferCommandPool = VulkanCommandPool(m_Device, m_Device.getTransferQueueIndex());
		m_SwapChain = VulkanSwapChain(m_Device, m_Surface);


		// Render pass
		std::vector<VkAttachmentDescription> renderPassAttachments;
		renderPassAttachments.push_back(VulkanRenderPass::createAttachmentDesc(m_SwapChain.getFormat()));

		std::vector<VkAttachmentReference> renderPassReferences;
		renderPassReferences.push_back(VulkanRenderPass::createAttachmentRef(0));
	
		std::vector<VkSubpassDescription> renderPassSubpasses;
		renderPassSubpasses.push_back(VulkanRenderPass::createSubpass(0, nullptr, renderPassReferences.size(), renderPassReferences.data()));

		std::vector<VkSubpassDependency> renderPassDependencies;
		renderPassDependencies.push_back(VulkanRenderPass::createDependency());

		m_RenderPass = VulkanRenderPass(m_Device, renderPassAttachments.size(), renderPassAttachments.data(), renderPassSubpasses.size(), renderPassSubpasses.data(), renderPassDependencies.size(), renderPassDependencies.data());


		// Framebuffers
		m_Framebuffers.resize(m_SwapChain.getImageCount());
		for (U32 i = 0; i < m_Framebuffers.size(); i++)
		{
			m_Framebuffers[i] = VulkanFramebuffer(m_Device, m_SwapChain.getImageViews()[i], m_SwapChain.getExtent().width, m_SwapChain.getExtent().height, m_RenderPass);
		}


		// Uniform buffer
		m_UniformBuffers.resize(m_SwapChain.getImageCount());
		for (U32 i = 0; i < m_UniformBuffers.size(); i++)
		{
			m_UniformBuffers[i] = VulkanUniformBuffer(m_Device, sizeof(UniformBufferObject));
		}


		// Descriptor pool
		VkDescriptorPoolSize uniformBufferDescriptorPoolSize{ 
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			m_SwapChain.getImageCount()
		};
		m_DescriptorPool = VulkanDescriptorPool(m_Device, m_SwapChain.getImageCount(), 1, &uniformBufferDescriptorPoolSize);


		// Descriptor set layout binding
		VkDescriptorSetLayoutBinding uniformDesc = VulkanDescriptorSetLayout::createDescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT);
		
		
		// Descriptor set layout
		m_DescriptorSetLayout = VulkanDescriptorSetLayout(m_Device, 1, &uniformDesc);
		

		// Descriptor sets
		m_DescriptorSets.resize(m_SwapChain.getImageCount());
		for (U32 i = 0; i < m_DescriptorSets.size(); i++)
		{
			m_DescriptorSets[i] = VulkanDescriptorSet(m_Device, m_DescriptorPool, m_DescriptorSetLayout);
		}

		for (size_t i = 0; i < m_SwapChain.getImageCount(); i++)
		{
			VkDescriptorBufferInfo descriptorBufferInfo;
			descriptorBufferInfo.buffer = m_UniformBuffers[i].getBuffer();
			descriptorBufferInfo.offset = 0;
			descriptorBufferInfo.range = sizeof(UniformBufferObject);

			VkWriteDescriptorSet descriptorWrite{};

			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.pNext = nullptr;
			descriptorWrite.dstSet = m_DescriptorSets[i].getSet();
			descriptorWrite.dstBinding = 0; // Binding 0 in shader
			descriptorWrite.dstArrayElement = 0; // Descriptors can also be arrays so 0th index is given
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.pImageInfo = nullptr; // Optional
			descriptorWrite.pBufferInfo = &descriptorBufferInfo;
			descriptorWrite.pTexelBufferView = nullptr; // Optional

			vkUpdateDescriptorSets(m_Device.getDevice(), 1, &descriptorWrite, 0, nullptr);
		}


		// Vertex buffer
		struct Vertex
		{
			Froth::Math::Vector3D position;
			Froth::Math::Vector3D color;
		};
		std::vector<Vertex> vertices = {
			{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
			{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
			{{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
			{{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}
		};
		m_VertexBuffer = VulkanVertexBuffer(m_Device, sizeof(Vertex), vertices.size());
		m_VertexBuffer.updateData(vertices.size(), sizeof(Vertex), vertices.data());
		
		// Index buffer
		std::vector<U16> indices = { 
			1, 2, 3,
			4, 7, 6,
			4, 5, 1,
			1, 5, 6,
			6, 7, 3,
			4, 0, 3,
			0, 1, 3,
			5, 4, 6,
			0, 4, 6,
			2, 1, 6,
			2, 6, 3,
			7, 4, 3
		};
		m_IndexBuffer = VulkanIndexBuffer(m_Device, indices.size());
		m_IndexBuffer.updateData(indices.size(), indices.data());

		// Vertex shader module
		std::ifstream vertfile("shaders/vert.spv", std::ios::ate | std::ios::binary);
		if (!vertfile.is_open()) std::cout << "failed to open file" << std::endl;

		size_t vertfileSize = (size_t) vertfile.tellg();
		std::vector<char> vertexShaderSrc(vertfileSize);
		vertfile.seekg(0);
		vertfile.read(vertexShaderSrc.data(), vertfileSize);
		vertfile.close();
		
		m_VertexShaderModule = VulkanShaderModule(m_Device, vertexShaderSrc.size(), vertexShaderSrc.data());


		// Fragment shader module
		std::ifstream fragfile("shaders/frag.spv", std::ios::ate | std::ios::binary); //
		if (!fragfile.is_open()) std::cout << "failed to open file" << std::endl;

		size_t fragfileSize = (size_t) fragfile.tellg();
		std::vector<char> fragmentShaderSrc(fragfileSize);
		fragfile.seekg(0);
		fragfile.read(fragmentShaderSrc.data(), fragfileSize);
		fragfile.close();

		m_FragmentShaderModule = VulkanShaderModule(m_Device, fragmentShaderSrc.size(), fragmentShaderSrc.data());


		// Pipeline layout
		m_PipelineLayout = VulkanPipelineLayout(m_Device, 1, &m_DescriptorSetLayout);


		// Graphics pipeline
		std::vector<VkPipelineShaderStageCreateInfo> pipelineShaderStages = {
			VulkanGraphicsPipeline::createShaderStageInfo(VK_SHADER_STAGE_VERTEX_BIT, m_VertexShaderModule),
			VulkanGraphicsPipeline::createShaderStageInfo(VK_SHADER_STAGE_FRAGMENT_BIT, m_FragmentShaderModule) 
		};

		std::vector<VkVertexInputBindingDescription> pipelineBindingDescs = { VulkanGraphicsPipeline::createVertexInputBindingDescription(0, sizeof(Vertex)) };
		std::vector<VkVertexInputAttributeDescription> pipelineAttributeDescs = { 
			VulkanGraphicsPipeline::createVertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)),
			VulkanGraphicsPipeline::createVertexInputAttributeDescription(1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color))
		};
		VkPipelineVertexInputStateCreateInfo pipelineVertexInputState = VulkanGraphicsPipeline::createVertexInputStateInfo(pipelineBindingDescs.size(), pipelineBindingDescs.data(), pipelineAttributeDescs.size(), pipelineAttributeDescs.data());

		VkPipelineInputAssemblyStateCreateInfo pipelineAssemblyState = VulkanGraphicsPipeline::createInputAssemblyStateInfo();

		VkViewport viewport{
			0.0f, // x
			0.0f, // y
			m_SwapChain.getExtent().width, // width
			m_SwapChain.getExtent().height, // height
			0.0f, // minDepth
			1.0f // maxDepth
		};
		VkRect2D scissor{
			{0, 0}, // offset
			m_SwapChain.getExtent() // extent
		};
		VkPipelineViewportStateCreateInfo pipelineViewportState = VulkanGraphicsPipeline::createViewportStateInfo(1, &viewport, 1, &scissor);
		
		VkPipelineRasterizationStateCreateInfo pipelineRasterizationState = VulkanGraphicsPipeline::createRasterizationStateInfo(VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FALSE);

		VkPipelineMultisampleStateCreateInfo pipelineMultisampleState = VulkanGraphicsPipeline::createMultisampleStateInfo(VK_SAMPLE_COUNT_1_BIT);

		F32 colorBlendConstants[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		std::vector<VkPipelineColorBlendAttachmentState> colorBlendStateAttachments = { VulkanGraphicsPipeline::createColorBlendAttachmentStateInfo(VK_FALSE, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, (VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)) };
		VkPipelineColorBlendStateCreateInfo pipelineColorBlendState = VulkanGraphicsPipeline::createColorBlendStateInfo(VK_FALSE, VK_LOGIC_OP_COPY, colorBlendStateAttachments.size(), colorBlendStateAttachments.data(), colorBlendConstants);

		std::vector<VkDynamicState> pipelineDynamicStates = { VK_DYNAMIC_STATE_VIEWPORT };
		VkPipelineDynamicStateCreateInfo pipelineDynamicState = VulkanGraphicsPipeline::createDynamicStateInfo(pipelineDynamicStates.size(), pipelineDynamicStates.data());

		m_GraphicsPipeline = VulkanGraphicsPipeline(m_Device,
			pipelineShaderStages.size(), pipelineShaderStages.data(),
			&pipelineVertexInputState,
			&pipelineAssemblyState,
			nullptr,
			&pipelineViewportState,
			&pipelineRasterizationState,
			&pipelineMultisampleState,
			nullptr,
			&pipelineColorBlendState,
			nullptr/*&pipelineDynamicState*/,
			m_PipelineLayout,
			m_RenderPass,
			0,
			VK_NULL_HANDLE,
			0
		);


		// Command buffers
		m_GraphicsCommandBuffers.resize(m_SwapChain.getImageCount());
		for (U32 i = 0; i < m_GraphicsCommandBuffers.size(); i++)
		{
			m_GraphicsCommandBuffers[i] = VulkanCommandBuffer(m_Device, m_GraphicsCommandPool);
		}

		for (U32 i = 0; i < m_GraphicsCommandBuffers.size(); i++)
		{
			m_GraphicsCommandBuffers[i].begin();
			VkRect2D renderArea{
				{0,0},
				m_SwapChain.getExtent()
			};
			m_GraphicsCommandBuffers[i].beginRenderPass(m_RenderPass, m_Framebuffers[i], renderArea);
			m_GraphicsCommandBuffers[i].bindGraphicsPipeline(m_GraphicsPipeline);
 			m_GraphicsCommandBuffers[i].bindVertexBuffer(m_VertexBuffer);
			m_GraphicsCommandBuffers[i].bindIndexBuffer(m_IndexBuffer);
 			m_GraphicsCommandBuffers[i].bindGraphicsDescriptorSets(m_PipelineLayout, 1, &m_DescriptorSets[i]);
			m_GraphicsCommandBuffers[i].drawIndexed(indices.size());
			m_GraphicsCommandBuffers[i].endRenderPass();
			m_GraphicsCommandBuffers[i].end();
		}

		m_CmdSubmitSemaphores.resize(m_SwapChain.getImageCount());
		m_PresentSemaphores.resize(m_SwapChain.getImageCount());
		m_FrameFinishedFences.resize(m_SwapChain.getImageCount());
		m_FrameInProgressFences.resize(m_SwapChain.getImageCount(), VK_NULL_HANDLE);
		for (U32 i = 0; i < m_CmdSubmitSemaphores.size(); i++)
		{
			m_CmdSubmitSemaphores[i] = VulkanSemaphore(m_Device);
			m_PresentSemaphores[i] = VulkanSemaphore(m_Device);
			m_FrameFinishedFences[i] = VulkanFence(m_Device);
		}
	}

	VulkanRenderer::~VulkanRenderer()
	{
		vkDeviceWaitIdle(m_Device.getDevice());
	}

	void VulkanRenderer::onUpdate()
	{
		vkWaitForFences(m_Device.getDevice(), 1, &m_FrameFinishedFences[m_CurrentFrame].getFence(), VK_TRUE, UINT64_MAX);

		U32 swapImageIndex;
		vkAcquireNextImageKHR(m_Device.getDevice() , m_SwapChain.getSwapchain(), 0, m_CmdSubmitSemaphores[m_CurrentFrame].getSemaphore(), VK_NULL_HANDLE, &swapImageIndex);

		if (m_FrameInProgressFences[swapImageIndex] != VK_NULL_HANDLE) // The image that we want to render to is currently in use
		{
			vkWaitForFences(m_Device.getDevice(), 1, &m_FrameInProgressFences[swapImageIndex], VK_TRUE, UINT64_MAX);
		}

		// Mark the swap chain image as in use by this frame
		// The next frame that uses this image will wait on this frame to be done
		m_FrameInProgressFences[swapImageIndex] = m_FrameFinishedFences[m_CurrentFrame].getFence();


		
		UniformBufferObject ubo { 
			Froth::Math::makeTranslation(0.0f, 0.0f, 0.0f),
			Froth::Math::Transform4D::inverse(Froth::Math::makeRotationY(Froth::Math::toRadians(30.0f)) * Froth::Math::makeRotationX(Froth::Math::toRadians(-30.0f))*Froth::Math::makeTranslation(0.0f, -0.5f, -4.0f)),
			Froth::Math::makeFrustumProjection(Froth::Math::toRadians(60.0f), 4.0f/3.0f, 0.01f, 10.0f)
		};

		m_UniformBuffers[swapImageIndex].updateData(sizeof(UniformBufferObject), &ubo);

		VkSubmitInfo submitInfo;
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pNext = nullptr;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &m_CmdSubmitSemaphores[m_CurrentFrame].getSemaphore();
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_GraphicsCommandBuffers[swapImageIndex].getCommandBuffer();
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_PresentSemaphores[m_CurrentFrame].getSemaphore();

		vkResetFences(m_Device.getDevice(), 1, &m_FrameFinishedFences[m_CurrentFrame].getFence());
		if (vkQueueSubmit(m_Device.getGraphicsQueue(), 1, &submitInfo, m_FrameFinishedFences[m_CurrentFrame].getFence()) != VK_SUCCESS) std::cout << "Failed to submit draw command!" << std::endl;

		VkPresentInfoKHR presentInfo;
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = nullptr;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &m_PresentSemaphores[m_CurrentFrame].getSemaphore();
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_SwapChain.getSwapchain();
		presentInfo.pImageIndices = &swapImageIndex; // Index of image on swap chain to present to // Array for presenting to multiple swap chains
		presentInfo.pResults = nullptr; // Array of VkResult to check for successful presentation, only needed with multiple swap chains

		if (vkQueuePresentKHR(m_Device.getPresentQueue(), &presentInfo) != VK_SUCCESS)
		{
			std::cout << "failed to present swap chain image" << std::endl;
		}

		m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	VkPhysicalDevice VulkanRenderer::selectPhysicalDevice()
	{
		// TODO: Take as parameter // HACK: Must be updated with VulkanDevice Class
		const std::vector<const char*> requiredExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		U32 availableDeviceCount = 0;
		vkEnumeratePhysicalDevices(VulkanInstance::get().getInstance(), &availableDeviceCount, nullptr);

		// TODO: Assert
		if (availableDeviceCount == 0) std::cout << "No Vulkan capable devices available!" << std::endl;

		std::vector<VkPhysicalDevice> availableDevices(availableDeviceCount);
		vkEnumeratePhysicalDevices(VulkanInstance::get().getInstance(), &availableDeviceCount, availableDevices.data());

		struct deviceVals
		{
			size_t index = 0;
			bool discrete = false;
			U32 score = 0;
		} deviceVals;

		bool suitableDeviceExists = false;

		for (U32 i  = 0; i < availableDevices.size(); i++)
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(availableDevices[i], &deviceProperties);
			
			bool supportsRequiredExtensions = checkDeviceExtensionsSupport(availableDevices[i], requiredExtensions);
			if (!supportsRequiredExtensions) continue; // These extensions must be supported
			
			U32 queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(availableDevices[i], &queueFamilyCount, nullptr);
			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(availableDevices[i], &queueFamilyCount, queueFamilies.data());

			bool graphicsSupport = false;
			bool transferSupport = false;
			bool computeSupport = false;
			bool presentSupport = false;

			for (U32 j = 0; j < queueFamilies.size();j++)
			{
				if (queueFamilies[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) graphicsSupport = true;

				VkBool32 surfaceSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(availableDevices[i], i, m_Surface.getSurface(), &surfaceSupport);
				if (surfaceSupport) presentSupport = true;

				if (queueFamilies[j].queueFlags & VK_QUEUE_TRANSFER_BIT) transferSupport = true;
				if (queueFamilies[j].queueFlags & VK_QUEUE_COMPUTE_BIT) computeSupport = true;
			}
			
			if (!graphicsSupport || !transferSupport || !presentSupport || !computeSupport) continue; // Must be able to render graphics, present to screen, and support transferring memory

			VkPhysicalDeviceFeatures availableDeviceFeatures;
			vkGetPhysicalDeviceFeatures(availableDevices[i], &availableDeviceFeatures);
			// HACK: Must manually add any required features to this check
			bool requiredFeatureSupport = (availableDeviceFeatures.geometryShader && availableDeviceFeatures.tessellationShader && availableDeviceFeatures.samplerAnisotropy);
			if(!requiredFeatureSupport) continue; // Must support the required features
			suitableDeviceExists = true;

			U32 optionalFeatureSupportCount = 0;
			// TODO: Increase optional feature support count for each available feature

			// TODO: Add all points to final score
			U32 deviceScore = optionalFeatureSupportCount;

			if (!deviceVals.discrete)
			{
				if (deviceProperties.deviceType & VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) // Prefer discrete
				{
					deviceVals.discrete = true;
					deviceVals.index = i;
					deviceVals.score = deviceScore;
				}
				else if(deviceVals.score < deviceScore)
				{
					deviceVals.discrete = false;
					deviceVals.index = i;
					deviceVals.score = deviceScore;
				}
				// TODO: Add preference for other device types
			}

		}
		// TODO: Assert
		if (!suitableDeviceExists) std::cout << "No suitable Vulkan device was found!" << std::endl;
		return availableDevices[deviceVals.index];
	}

	bool VulkanRenderer::checkDeviceExtensionsSupport(const VkPhysicalDevice& device, const std::vector<const char*>& extensions)
	{
		// TODO: Replace with counter to check if all are supported and return U32

		U32 availableExtensionCount = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &availableExtensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &availableExtensionCount, availableExtensions.data());

		std::vector<bool> results(extensions.size(), false);

		for (VkExtensionProperties e : availableExtensions)
		{
			for (size_t i = 0; i < extensions.size(); i++)
			{
				if (strcmp(e.extensionName, extensions[i]) == 0)
				{
					results[i] = true;
				}

			}
		}
		
		for (bool r : results)
		{
			if (r == false) return false;
		}
		return true;
	}

}