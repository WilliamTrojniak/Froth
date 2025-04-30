#pragma once
#include "src/renderer/vulkan/VulkanDevice.h"
#include "src/renderer/vulkan/VulkanSurface.h"
#include <vector>

namespace Froth {

class VulkanSwapChain {
public:
  VulkanSwapChain(const VulkanSurface &surface, const VulkanSwapChain *oldSwapchain);
  ~VulkanSwapChain();
  VkExtent2D extent() const { return m_Extent; }
  VkSurfaceFormatKHR format() const { return m_Format; }

  const std::vector<VkImageView> views() const { return m_ImageViews; }
  operator VkSwapchainKHR() const { return m_Swapchain; }

private:
  VkExtent2D m_Extent;
  VkSurfaceFormatKHR m_Format;
  VkSwapchainKHR m_Swapchain;
  std::vector<VkImage> m_Images;
  std::vector<VkImageView> m_ImageViews;

private:
  static VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
  static VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> &availableModes);
  static VkExtent2D chooseExtent(const VulkanSurface &surface, const VulkanDevice::SurfaceCapabilities &capabilities);
};

} // namespace Froth
