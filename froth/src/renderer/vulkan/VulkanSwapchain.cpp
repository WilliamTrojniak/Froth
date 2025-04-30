#include "VulkanSwapchain.h"
#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include "src/core/logger/Logger.h"
#include <algorithm>
#include <cstdint>

namespace Froth {

VulkanSwapChain::VulkanSwapChain(const VulkanDevice &device, const VulkanSurface &surface, const VulkanSwapChain *oldSwapchain)
    : m_Device(device) {
  VulkanDevice::SurfaceCapabilities surfaceCapabilities = m_Device.getSurfaceSupport(surface);

  // TODO: Maybe this is chosen by the user?
  m_Format = chooseSurfaceFormat(surfaceCapabilities.formats);
  VkPresentModeKHR presentMode = choosePresentMode(surfaceCapabilities.presentModes);
  m_Extent = chooseExtent(surface, surfaceCapabilities);

  uint32_t imageCount = surfaceCapabilities.capabilities.minImageCount + 1; // To avoid waiting on driver internal operations

  if (surfaceCapabilities.capabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.capabilities.maxImageCount) {
    // If maxImageCount = 0, then unlimited
    imageCount = surfaceCapabilities.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = m_Format.format;
  createInfo.imageColorSpace = m_Format.colorSpace;
  createInfo.imageExtent = m_Extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  VulkanDevice::QueueFamilies indices = m_Device.getQueueFamilies();
  uint32_t queueFamilyIndices[] = {indices.graphics.index, indices.present.index};

  if (indices.graphics.index != indices.present.index) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = nullptr;
  }

  createInfo.preTransform = surfaceCapabilities.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  VkSwapchainKHR oldSwapchainPtr = VK_NULL_HANDLE;
  if (oldSwapchain) {
    oldSwapchainPtr = *oldSwapchain;
  }
  createInfo.oldSwapchain = oldSwapchainPtr;

  if (vkCreateSwapchainKHR(m_Device, &createInfo, m_Device.instance().allocator(), &m_Swapchain) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create swap chain");
  }

  if (vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &imageCount, nullptr) != VK_SUCCESS) {
    vkDestroySwapchainKHR(m_Device, m_Swapchain, m_Device.instance().allocator());
    m_Swapchain = nullptr;
    FROTH_ERROR("Failed to retreive created swap chain images");
  }

  m_Images.resize(imageCount);
  if (vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &imageCount, m_Images.data()) != VK_SUCCESS) {
    vkDestroySwapchainKHR(m_Device, m_Swapchain, m_Device.instance().allocator());
    m_Swapchain = nullptr;
    FROTH_ERROR("Failed to retreive created swap chain images");
  }

  m_ImageViews.resize(imageCount);
  for (uint32_t i = 0; i < imageCount; i++) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = m_Images[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = m_Format.format;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(m_Device, &createInfo, m_Device.instance().allocator(), &m_ImageViews[i]) != VK_SUCCESS) {
      for (uint32_t k = 0; k < i; k++) {
        vkDestroyImageView(m_Device, m_ImageViews[k], m_Device.instance().allocator());
        m_ImageViews[i] = nullptr;
      }
      vkDestroySwapchainKHR(m_Device, m_Swapchain, m_Device.instance().allocator());
      m_Swapchain = nullptr;
      FROTH_ERROR("Failed to create swapchain image view");
    }
  }
}

VulkanSwapChain::~VulkanSwapChain() {
  for (auto imageView : m_ImageViews) {
    vkDestroyImageView(m_Device, imageView, m_Device.instance().allocator());
    FROTH_DEBUG("Destroyed Vulkan Swapchain Image View")
  }

  if (m_Swapchain) {
    vkDestroySwapchainKHR(m_Device, m_Swapchain, m_Device.instance().allocator());
    m_Swapchain = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Swapchain");
  }
}

VkSurfaceFormatKHR VulkanSwapChain::chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
  // TODO: Improve the selection algorithm
  // This is very rudimentary right now
  for (const auto &format : availableFormats) {
    if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return format;
    }
  }
  return availableFormats[0];
}

VkPresentModeKHR VulkanSwapChain::choosePresentMode(const std::vector<VkPresentModeKHR> &availableModes) {
  // TODO: Improve the selection algorithm
  for (const auto &mode : availableModes) {
    if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
      return mode;
  }

  return VK_PRESENT_MODE_FIFO_KHR; // Guaranteed to be available
}

VkExtent2D VulkanSwapChain::chooseExtent(const VulkanSurface &surface, const VulkanDevice::SurfaceCapabilities &capabilities) {
  if (capabilities.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capabilities.capabilities.currentExtent;
  }

  VkExtent2D actualExtent;
  surface.getFramebufferSize(actualExtent.width, actualExtent.height);

  actualExtent.width = std::clamp(actualExtent.width, capabilities.capabilities.minImageExtent.width, capabilities.capabilities.maxImageExtent.width);
  actualExtent.height = std::clamp(actualExtent.height, capabilities.capabilities.minImageExtent.height, capabilities.capabilities.maxImageExtent.height);
  return actualExtent;
}

} // namespace Froth
