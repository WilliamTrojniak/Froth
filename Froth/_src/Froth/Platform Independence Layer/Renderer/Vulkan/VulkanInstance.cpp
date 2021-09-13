#include <frothpch.h>
#include "VulkanInstance.h"

namespace Froth
{
	
	VulkanInstance::~VulkanInstance()
	{
		std::cout << "VulkanInstance destroyed" << std::endl;
		vkDestroyInstance(m_Instance, nullptr);
	}

	VulkanInstance::VulkanInstance()
	{
		std::cout << "RUN" << std::endl;

		VkApplicationInfo appInfo;
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pNext = nullptr;
		appInfo.pApplicationName = "Application"; // TODO: Take as parameter
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // TODO: Take as parameter
		appInfo.pEngineName = FROTH_ENGINE_NAME;
		appInfo.engineVersion = VK_MAKE_VERSION(FROTH_VERSION_MAJOR, FROTH_VERSION_MINOR, FROTH_VERSION_REVISION); // TODO: Take as parameter
		appInfo.apiVersion = VK_API_VERSION_1_0;

		U32 availableLayerCount = 0;
		std::vector<VkLayerProperties> availableLayers;
		vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr);
		availableLayers.resize(availableLayerCount);
		vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers.data());
		std::vector<char*> desiredLayers;
#ifdef FROTH_ENABLE_VKVALIDATIONLAYER
		desiredLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif // FROTH_ENABLE_VKVALIDATIONLAYER
		// TODO: Take additional layers as parameters
		for (size_t i = 0; i < desiredLayers.size(); i++)
		{
			for (size_t j = 0; j < availableLayerCount; j++)
			{
				
				if (strcmp(desiredLayers[i], availableLayers[j].layerName) == 0) // Desired and available layer matches
				{
					m_Layers.push_back(desiredLayers[i]);
				}
			}
		}

		uint32_t availableExtensionCount = 0;
		std::vector<VkExtensionProperties> availableExtensions;
		vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);
		availableExtensions.resize(availableExtensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());
		std::vector<char*> desiredExtensions(2);
		desiredExtensions[0] = VK_KHR_SURFACE_EXTENSION_NAME;
#ifdef FROTH_WINDOWAPI_WIN
		desiredExtensions[1] = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
#endif
		for (size_t i = 0; i < desiredExtensions.size(); i++)
		{
			for (size_t j = 0; j < availableExtensionCount; j++)
			{
				if (strcmp(desiredExtensions[i], availableExtensions[j].extensionName) == 0) // Desired and available extension matches
				{
					m_Extensions.push_back(desiredExtensions[i]);
				
				}
			}
		}


		VkInstanceCreateInfo instanceInfo;
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pNext = nullptr;
		instanceInfo.flags = 0;
		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledLayerCount = m_Layers.size();
		instanceInfo.ppEnabledLayerNames = m_Layers.data();
		instanceInfo.enabledExtensionCount = m_Extensions.size();
		instanceInfo.ppEnabledExtensionNames = m_Extensions.data();

		// TODO: Assert
		if (vkCreateInstance(&instanceInfo, nullptr, &m_Instance) != VK_SUCCESS) std::cout << "Failed to create Vulkan Instance!" << std::endl;
	}

}