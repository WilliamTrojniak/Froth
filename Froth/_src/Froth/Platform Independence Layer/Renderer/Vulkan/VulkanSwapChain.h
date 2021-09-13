#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanDevice.h"
#include "VulkanSurface.h"
#include "VulkanImageView.h"


namespace Froth
{
	class VulkanSwapChain
	{
	public:
		VulkanSwapChain() = default;
		VulkanSwapChain(const VulkanDevice& device, const VulkanSurface& surface);
		VulkanSwapChain(VulkanSwapChain& other) = delete;
		VulkanSwapChain(VulkanSwapChain&& other);
		~VulkanSwapChain();

		VulkanSwapChain& operator=(VulkanSwapChain&& other);

		const VkSwapchainKHR& getSwapchain() const { return m_SwapChain; }
		const VkFormat& getFormat() const { return m_Format.format; }
		const VkExtent2D& getExtent() const { return m_Extent; }
		const std::vector<VkImage>& getImages() const { return m_Images; }
		U32 getImageCount() const;
		const std::vector<VulkanImageView>& getImageViews() const { return m_ImageViews; }

	private:
		VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) const;

		std::vector<VkImage> retrieveImages();
		std::vector<VulkanImageView> createImagesViews();

		const VulkanDevice* m_Device; // HACK: Device not owned by swap chain, used for cleanup
		VkSwapchainKHR m_SwapChain;
		VkSurfaceFormatKHR m_Format;
		VkExtent2D m_Extent;

		std::vector<VkImage> m_Images;
		std::vector<VulkanImageView> m_ImageViews;
	};
}