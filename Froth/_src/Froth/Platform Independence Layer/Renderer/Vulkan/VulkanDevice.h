#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Froth/Platform Independence Layer/Renderer/Vulkan/VulkanSurface.h"

namespace Froth
{
	class VulkanDevice
	{
	private:
		struct QueueTypeIndices
		{
			U32 present, graphics, transfer, compute;
		};

		VkDevice m_LogicDevice;
		VkPhysicalDevice m_PhysicalDevice;

		VkPhysicalDeviceFeatures m_EnabledFeatures;
		VkPhysicalDeviceMemoryProperties m_MemoryProperties;

		// TODO: Change to vectors for each queue type
		VkQueue m_PresentQueue;
		VkQueue m_GraphicsQueue;
		VkQueue m_ComputeQueue;
		VkQueue m_TransferQueue;
		

	public:
		VulkanDevice() = default;
		VulkanDevice(VulkanDevice& other) = delete;
		VulkanDevice(VulkanDevice&& other);
		VulkanDevice(VkPhysicalDevice physicalDevice, const VulkanSurface& surface);
		~VulkanDevice();
		VulkanDevice& operator=(VulkanDevice&& other);

		const VkDevice& getDevice() const { return m_LogicDevice; }
		U32 getPresentQueueIndex() const { return m_QueueIndices.present; }
		U32 getGraphicsQueueIndex() const { return m_QueueIndices.graphics; }
		U32 getComputeQueueIndex() const { return m_QueueIndices.compute; }
		U32 getTransferQueueIndex() const { return m_QueueIndices.transfer; }
		const VkQueue& getPresentQueue() const { return m_PresentQueue; }
		const VkQueue& getGraphicsQueue() const { return m_GraphicsQueue; }
		const VkQueue& getComputeQueue() const { return m_ComputeQueue; }
		const VkQueue& getTransferQueue() const { return m_TransferQueue; }

		VkSurfaceCapabilitiesKHR getSurfaceCapabilities(const VulkanSurface& surface) const;
		std::vector<VkSurfaceFormatKHR> getSurfaceFormats(const VulkanSurface& surface) const;
		const VkPhysicalDeviceMemoryProperties& getMemoryProperties() const { return m_MemoryProperties; }

	private:
		
		QueueTypeIndices getQueueTypeIndices(const VulkanSurface& surface) const;
		std::map<U32, U8> getQueueIndexRepeatCount();

		VkDeviceQueueCreateInfo createQueueInfo(U32 queueIndex, U32 queueCount, F32* priorities) const;

		QueueTypeIndices m_QueueIndices;

		U32 m_MaxFamilyQueueCount = 0;
		const U8 m_PRESENT_QUEUE_COUNT = 1;
		const U8 m_GRAPHICS_QUEUE_COUNT = 1;
		const U8 m_COMPUTE_QUEUE_COUNT = 1;
		const U8 m_TRANSFER_QUEUE_COUNT = 1;
	};
}