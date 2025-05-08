#pragma once
#include "VulkanDevice.h"
#include "VulkanSurface.h"
#include "vulkan/vulkan_core.h"
#include <vector>

namespace Froth {

class VulkanSwapchain {
public:
  VulkanSwapchain() = default;

  static VulkanSwapchain create(const VulkanSurface &surface, const VulkanSwapchain *oldSwapChainPtr);
  VulkanSwapchain(const VulkanSurface &surface, const VkSurfaceFormatKHR &format, const VkExtent2D &extent, VkPresentModeKHR presentMode, VkSurfaceTransformFlagBitsKHR transform, uint32_t imageCount, const VulkanSwapchain *oldSwapchain);
  ~VulkanSwapchain();

  VulkanSwapchain(const VulkanSwapchain &) = delete;
  VulkanSwapchain &operator=(const VulkanSwapchain &) = delete;
  VulkanSwapchain(VulkanSwapchain &&);
  VulkanSwapchain &operator=(VulkanSwapchain &&);

  VkSurfaceFormatKHR format() const { return m_Format; }
  const VkExtent2D &extent() const { return m_Extent; }

  const std::vector<VkImageView> views() const { return m_ImageViews; }
  operator VkSwapchainKHR() const { return m_Swapchain; }

private:
  VkSurfaceFormatKHR m_Format;
  VkSwapchainKHR m_Swapchain = nullptr;
  VkExtent2D m_Extent{}; // May differ from surface's
  std::vector<VkImage> m_Images{};
  std::vector<VkImageView> m_ImageViews{};

private:
  static VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
  static VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> &availableModes);
  static VkExtent2D chooseExtent(const VulkanSurface &surface, const VulkanDevice::SurfaceCapabilities &capabilities);
};

} // namespace Froth
