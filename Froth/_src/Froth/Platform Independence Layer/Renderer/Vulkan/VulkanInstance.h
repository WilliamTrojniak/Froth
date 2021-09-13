#pragma once
#define GLFW_INCLUDE_VULKAN
#ifdef FROTH_WINDOWAPI_WIN
#define VK_USE_PLATFORM_WIN32_KHR
#endif // FROTH_WINDOWAPI_WIN

#include <GLFW/glfw3.h>
#include <vector>

namespace Froth
{
	// Singleton class
	class VulkanInstance
	{
	private:
		VkInstance m_Instance;
		std::vector<char*> m_Layers;
		std::vector<char*> m_Extensions;
	
	public: 
		~VulkanInstance();

		static VulkanInstance& get()
		{
			static VulkanInstance instance;
			return instance;
		}

		const VkInstance& getInstance() const { return m_Instance; }
		const std::vector<char*>& getLayers() const { return m_Layers; }
		const std::vector<char*>& getExtensions() const { return m_Extensions; }


		VulkanInstance(VulkanInstance const&) = delete;
		void operator=(VulkanInstance const&) = delete;

	private:
		VulkanInstance();

	};
}