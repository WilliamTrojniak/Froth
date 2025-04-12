#pragma once
#include "platform/window/Window.h"
#include "renderer/vulkan/VulkanDevice.h"
#include "renderer/vulkan/VulkanSurface.h"
#include <vector>

namespace Froth {

class VulkanSwapChain {
public:
  VulkanSwapChain(const VulkanDevice &device, const Window &window, const VulkanSurface &surface);
  ~VulkanSwapChain();
  VkExtent2D extent() const { return m_Extent; }

private:
  const VulkanDevice &m_Device;
  VkExtent2D m_Extent;
  VkSurfaceFormatKHR m_Format;
  VkSwapchainKHR m_Swapchain;
  std::vector<VkImage> m_Images;
  std::vector<VkImageView> m_ImageViews;

private:
  static VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
  static VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> &availableModes);
  static VkExtent2D chooseExtent(const VulkanDevice::SurfaceCapabilities &capabilities, const Window &window);
};

} // namespace Froth
