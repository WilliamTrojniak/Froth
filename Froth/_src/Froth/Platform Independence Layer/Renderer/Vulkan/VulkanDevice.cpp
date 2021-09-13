#include <frothpch.h>
#include "VulkanDevice.h"

namespace Froth
{
	VulkanDevice::VulkanDevice(VkPhysicalDevice physicalDevice, const VulkanSurface& surface)
		: m_PhysicalDevice(physicalDevice), m_EnabledFeatures({ VK_FALSE })
	{
		vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &m_MemoryProperties);

		const std::vector<const char*> requiredExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME }; // HACK: Any updates made here must also be made in VulkanRenderer.cpp

		


		m_QueueIndices = getQueueTypeIndices(surface);

		std::map<U32, U8> queueCounts = getQueueIndexRepeatCount();

		std::vector<VkDeviceQueueCreateInfo> queueInfos(queueCounts.size());

		std::vector<F32> priorities(m_MaxFamilyQueueCount, 1.0f); // TODO: Individual priorities

		U32 queueIndex = 0;
		for (auto queueFamily : queueCounts)
		{
			queueInfos[queueIndex] = createQueueInfo(queueFamily.first, queueFamily.second, priorities.data());
			queueIndex++;
		}

		VkPhysicalDeviceFeatures availableDeviceFeatures;
		vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &availableDeviceFeatures);
		// HACK: Required features must be manually added here and in the device selection in Renderer.cpp
		m_EnabledFeatures.geometryShader = VK_TRUE;
		m_EnabledFeatures.tessellationShader = VK_TRUE;
		m_EnabledFeatures.samplerAnisotropy = VK_TRUE;

		// TODO: Enable any optional features by setting enabled features to the corresponding available features value
		// Thereby enabling the feature only if it is available.

		VkDeviceCreateInfo logicDeviceInfo
		{
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, //sType;
			nullptr, //pNext;
			0, //flags;
			queueInfos.size(), //queueCreateInfoCount
			queueInfos.data(), //pQueueCreateInfos;
			0, //enabledLayerCount;
			nullptr, //ppEnabledLayerName
			requiredExtensions.size(),//enabledExtensionCount
			requiredExtensions.data(),//ppEnabledExtension
			&m_EnabledFeatures//pEnabledFeatures;
		};

		// TODO: Assert
		if(vkCreateDevice(m_PhysicalDevice, &logicDeviceInfo, nullptr, &m_LogicDevice) != VK_SUCCESS) std::cout << "Failed to create Vulkan Logic Device" << std::endl;
		

		std::map<U32, U8> queueOffsets{};
		bool presentQueueCreated = false;
		bool graphicsQueueCreated = false;
		bool computeQueueCreated = false;
		bool transferQueueCreated = false;
		for (auto queueIndex : queueCounts)
		{
			queueOffsets[queueIndex.first] = 0;
		}

		for (auto queueIndex : queueCounts)
		{
			if (queueIndex.first == m_QueueIndices.present && !presentQueueCreated)
			{
				vkGetDeviceQueue(m_LogicDevice, queueIndex.first, queueOffsets[queueIndex.first], &m_PresentQueue);
				queueOffsets[queueIndex.first] += m_PRESENT_QUEUE_COUNT;
			}
			else if (queueIndex.first == m_QueueIndices.graphics && !graphicsQueueCreated)
			{
				vkGetDeviceQueue(m_LogicDevice, queueIndex.first, queueOffsets[queueIndex.first], &m_GraphicsQueue);
				queueOffsets[queueIndex.first] += m_GRAPHICS_QUEUE_COUNT;
			}
			else if (queueIndex.first == m_QueueIndices.compute && !computeQueueCreated)
			{
				vkGetDeviceQueue(m_LogicDevice, queueIndex.first, queueOffsets[queueIndex.first], &m_ComputeQueue);
				queueOffsets[queueIndex.first] += m_COMPUTE_QUEUE_COUNT;
			}
			else if (queueIndex.first == m_QueueIndices.transfer && !transferQueueCreated)
			{
				vkGetDeviceQueue(m_LogicDevice, queueIndex.first, queueOffsets[queueIndex.first], &m_TransferQueue);
				queueOffsets[queueIndex.first] += m_TRANSFER_QUEUE_COUNT;
			}
		}
	}

	VulkanDevice::VulkanDevice(VulkanDevice&& other)
	{
		m_LogicDevice = other.m_LogicDevice;
		m_PhysicalDevice = other.m_PhysicalDevice;
		m_EnabledFeatures = other.m_EnabledFeatures;
		m_MemoryProperties = other.m_MemoryProperties;
		m_PresentQueue = other.m_PresentQueue;
		m_GraphicsQueue = other.m_GraphicsQueue;
		m_ComputeQueue = other.m_ComputeQueue;
		m_TransferQueue = other.m_TransferQueue;
		m_QueueIndices = std::move(other.m_QueueIndices);

		other.m_LogicDevice = nullptr;
	}

	VulkanDevice::~VulkanDevice()
	{
		if (m_LogicDevice != nullptr)
		{
			std::cout << "Device destroyed" << std::endl;
			vkDestroyDevice(m_LogicDevice, nullptr);
		}
	}

	

	VulkanDevice& VulkanDevice::operator=(VulkanDevice&& other)
	{
		m_LogicDevice = other.m_LogicDevice;
		m_PhysicalDevice = other.m_PhysicalDevice;
		m_EnabledFeatures = other.m_EnabledFeatures;
		m_MemoryProperties = other.m_MemoryProperties;
		m_PresentQueue = other.m_PresentQueue;
		m_GraphicsQueue = other.m_GraphicsQueue;
		m_ComputeQueue = other.m_ComputeQueue;
		m_TransferQueue = other.m_TransferQueue;
		m_QueueIndices = std::move(other.m_QueueIndices);

		other.m_LogicDevice = nullptr;
		return *this;
	}

	VkSurfaceCapabilitiesKHR VulkanDevice::getSurfaceCapabilities(const VulkanSurface& surface) const
	{
		VkSurfaceCapabilitiesKHR surfaceCapabilites;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysicalDevice, surface.getSurface(), &surfaceCapabilites);

		return std::move(surfaceCapabilites);
	}

	std::vector<VkSurfaceFormatKHR> VulkanDevice::getSurfaceFormats(const VulkanSurface& surface) const
	{
		U32 surfaceFormatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, surface.getSurface(), &surfaceFormatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, surface.getSurface(), &surfaceFormatCount, surfaceFormats.data());
		return std::move(surfaceFormats);
	}

	VulkanDevice::QueueTypeIndices VulkanDevice::getQueueTypeIndices(const VulkanSurface& surface) const
	{
		U32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies.data());

		U32 presentIndex = -1;
		U32 graphicsIndex = -1;
		U32 computeIndex = -1;
		U32 transferIndex = -1;
		
		U32 secondaryPresentIndex = -1;
		U32 secondaryGraphicsIndex = -1;
		U32 secondaryComputeIndex = -1;
		U32 secondaryTransferIndex = -1;
		
		U32 tertiaryPresentIndex = -1;
		U32 tertiaryGraphicsIndex = -1;
		U32 tertiaryComputeIndex = -1;
		U32 tertiaryTransferIndex = -1;
		
		U32 fallbackPresentIndex = -1;
		U32 fallbackGraphicsIndex = -1;
		U32 fallbackComputeIndex = -1;
		U32 fallbackTransferIndex = -1;


		for (U32 i = 0; i < queueFamilyCount; i++)
		{
			VkBool32 surfaceSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i, surface.getSurface(), &surfaceSupport);
			
			bool graphicsSupport = (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT);
			bool computeSupport = (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT);
			bool transferSupport = (queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT);

			if (presentIndex == -1)
			{
				if (surfaceSupport && !(graphicsSupport || computeSupport || transferSupport)) presentIndex = i;
				else if (surfaceSupport && !(graphicsSupport || computeSupport)) secondaryPresentIndex = i;
				else if (surfaceSupport && !(graphicsSupport)) tertiaryPresentIndex = i;
				else if (surfaceSupport) fallbackPresentIndex = i; // TODO: Extend to avoid overloading onto last family
			}

			if (graphicsIndex == -1)
			{
				if (graphicsSupport && !(surfaceSupport || computeSupport || transferSupport)) graphicsIndex = i;
				else if (graphicsSupport && !(computeSupport || transferSupport)) secondaryGraphicsIndex = i;
				else if (graphicsSupport && !(computeSupport)) tertiaryGraphicsIndex = i;
				else if (graphicsSupport) fallbackGraphicsIndex = i; // TODO: Extend to avoid overloading onto last family
			}

			if (computeIndex == -1)
			{
				if (computeSupport && !(surfaceSupport || graphicsSupport || transferSupport)) computeIndex = i;
				else if (computeSupport && !(surfaceSupport || graphicsSupport)) secondaryComputeIndex = i; // Realistically, checking for no surface support means graphics are also not supported by the check for grahics support is left for clarity
				else if (computeSupport && !(graphicsSupport)) tertiaryComputeIndex = i;
				else if (computeSupport) fallbackComputeIndex = i; // TODO: Extend to avoid overloading onto last family
			}

			if (transferIndex == -1)
			{
				if (transferSupport && !(surfaceSupport || graphicsSupport || computeSupport)) transferIndex = i;
				else if (transferSupport && !(surfaceSupport || graphicsSupport)) secondaryTransferIndex = i;
				else if (transferSupport && !(graphicsSupport)) tertiaryTransferIndex = i;
				else if (transferSupport) fallbackTransferIndex = i; // TODO: Extend to avoid overloading onto last family
			}
			
		}

		if (presentIndex == -1)
		{
			if (secondaryPresentIndex != -1) presentIndex = secondaryPresentIndex;
			else if (tertiaryPresentIndex != -1) presentIndex = tertiaryPresentIndex;
			else if (fallbackPresentIndex != -1) presentIndex = fallbackPresentIndex;
		}

		if (graphicsIndex == -1)
		{
			if (secondaryGraphicsIndex != -1) graphicsIndex = secondaryGraphicsIndex;
			else if (tertiaryGraphicsIndex != -1) graphicsIndex = tertiaryGraphicsIndex;
			else if (fallbackGraphicsIndex != -1) graphicsIndex = fallbackGraphicsIndex;
		}

		if (computeIndex == -1)
		{
			if (secondaryComputeIndex != -1) computeIndex = secondaryComputeIndex;
			else if (tertiaryComputeIndex != -1) computeIndex = tertiaryComputeIndex;
			else if (fallbackComputeIndex != -1) computeIndex = fallbackComputeIndex;
		}

		if (transferIndex == -1)
		{
			if (secondaryTransferIndex != -1) transferIndex = secondaryTransferIndex;
			else if (tertiaryTransferIndex != -1) transferIndex = tertiaryTransferIndex;
			else if (fallbackTransferIndex != -1) transferIndex = fallbackTransferIndex;
		}

		return std::move(QueueTypeIndices{ presentIndex, graphicsIndex, computeIndex, transferIndex });

	}

	std::map<U32, U8> VulkanDevice::getQueueIndexRepeatCount()
	{
		//TODO: Check to make sure the requested number does not exceed max and consider requesting from same family for different type
		m_MaxFamilyQueueCount = max(m_PRESENT_QUEUE_COUNT, max(m_GRAPHICS_QUEUE_COUNT, max(m_COMPUTE_QUEUE_COUNT, m_TRANSFER_QUEUE_COUNT)));

		std::map<U32, U8> queueCounts{};
		if (queueCounts.find(m_QueueIndices.present) != queueCounts.end())
		{
			queueCounts[m_QueueIndices.present] += m_PRESENT_QUEUE_COUNT;
			m_MaxFamilyQueueCount = max(queueCounts[m_QueueIndices.present], m_MaxFamilyQueueCount);
		}
		else queueCounts[m_QueueIndices.present] = m_PRESENT_QUEUE_COUNT;
		if (queueCounts.find(m_QueueIndices.graphics) != queueCounts.end())
		{
			queueCounts[m_QueueIndices.graphics] += m_GRAPHICS_QUEUE_COUNT;
			m_MaxFamilyQueueCount = max(queueCounts[m_QueueIndices.graphics], m_MaxFamilyQueueCount);
		}
		else queueCounts[m_QueueIndices.graphics] = m_GRAPHICS_QUEUE_COUNT;
		if (queueCounts.find(m_QueueIndices.compute) != queueCounts.end())
		{
			queueCounts[m_QueueIndices.compute] += m_COMPUTE_QUEUE_COUNT;
			m_MaxFamilyQueueCount = max(queueCounts[m_QueueIndices.compute], m_MaxFamilyQueueCount);
		}
		else queueCounts[m_QueueIndices.compute] = m_COMPUTE_QUEUE_COUNT;
		if (queueCounts.find(m_QueueIndices.transfer) != queueCounts.end())
		{
			queueCounts[m_QueueIndices.transfer] += m_TRANSFER_QUEUE_COUNT;
			m_MaxFamilyQueueCount = max(queueCounts[m_QueueIndices.transfer], m_MaxFamilyQueueCount);
		}
		else queueCounts[m_QueueIndices.transfer] = m_TRANSFER_QUEUE_COUNT;
		return std::move(queueCounts);
	}

	VkDeviceQueueCreateInfo VulkanDevice::createQueueInfo(U32 queueIndex, U32 queueCount, F32* priorities) const
	{
		

		return std::move(VkDeviceQueueCreateInfo
		{
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, //sType
			nullptr, //pNext
			0, //flags
			queueIndex, //queueFamilyIndex
			queueCount, //queueCount
			priorities//pQueuePriorities
		});
	}

}