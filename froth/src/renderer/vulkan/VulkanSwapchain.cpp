#include "VulkanSwapchain.h"
#include "src/core/logger/Logger.h"
#include "src/renderer/vulkan/VulkanContext.h"
#include "src/renderer/vulkan/VulkanDevice.h"
#include <algorithm>
#include <cstdint>

namespace Froth {

VulkanSwapchain VulkanSwapchain::create(const VulkanSurface &surface, const VulkanSwapchain *oldSwapchainPtr) {
  VulkanContext &vctx = VulkanContext::get();

  VulkanDevice::SurfaceCapabilities surfaceCapabilities = VulkanDevice::physicalDeviceSurfaceSupport(vctx.physicalDevice(), surface);

  // TODO: Maybe this is chosen by the user?
  VkSurfaceFormatKHR format = chooseSurfaceFormat(surfaceCapabilities.formats);
  VkExtent2D extent = chooseExtent(surface, surfaceCapabilities);
  VkPresentModeKHR presentMode = choosePresentMode(surfaceCapabilities.presentModes);

  uint32_t imageCount = surfaceCapabilities.capabilities.minImageCount + 1; // To avoid waiting on driver internal operations

  if (surfaceCapabilities.capabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.capabilities.maxImageCount) {
    // If maxImageCount = 0, then unlimited
    imageCount = surfaceCapabilities.capabilities.maxImageCount;
  }

  return VulkanSwapchain(surface, format, extent, presentMode, surfaceCapabilities.capabilities.currentTransform, imageCount, oldSwapchainPtr);
}

VulkanSwapchain::VulkanSwapchain(const VulkanSurface &surface, const VkSurfaceFormatKHR &format, const VkExtent2D &extent, VkPresentModeKHR presentMode, VkSurfaceTransformFlagBitsKHR transform, uint32_t imageCount, const VulkanSwapchain *oldSwapchain)
    : m_Format(format), m_Extent(extent) {
  VulkanContext &vctx = VulkanContext::get();

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = m_Format.format;
  createInfo.imageColorSpace = m_Format.colorSpace;
  createInfo.imageExtent = m_Extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  VulkanDevice::QueueFamilies indices = vctx.device().getQueueFamilies();
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

  createInfo.preTransform = transform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  VkSwapchainKHR oldSwapchainPtr = VK_NULL_HANDLE;
  if (oldSwapchain) {
    oldSwapchainPtr = *oldSwapchain;
  }
  createInfo.oldSwapchain = oldSwapchainPtr;

  if (vkCreateSwapchainKHR(vctx.device(), &createInfo, vctx.allocator(), &m_Swapchain) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create swap chain");
  }

  if (vkGetSwapchainImagesKHR(vctx.device(), m_Swapchain, &imageCount, nullptr) != VK_SUCCESS) {
    vkDestroySwapchainKHR(vctx.device(), m_Swapchain, vctx.allocator());
    m_Swapchain = nullptr;
    FROTH_ERROR("Failed to retreive created swap chain images");
  }

  m_Images.resize(imageCount);
  if (vkGetSwapchainImagesKHR(vctx.device(), m_Swapchain, &imageCount, m_Images.data()) != VK_SUCCESS) {
    vkDestroySwapchainKHR(vctx.device(), m_Swapchain, vctx.allocator());
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

    if (vkCreateImageView(vctx.device(), &createInfo, vctx.allocator(), &m_ImageViews[i]) != VK_SUCCESS) {
      for (uint32_t k = 0; k < i; k++) {
        vkDestroyImageView(vctx.device(), m_ImageViews[k], vctx.allocator());
        m_ImageViews[i] = nullptr;
      }
      vkDestroySwapchainKHR(vctx.device(), m_Swapchain, vctx.allocator());
      m_Swapchain = nullptr;
      FROTH_ERROR("Failed to create swapchain image view");
    }
  }
}

VulkanSwapchain::VulkanSwapchain(VulkanSwapchain &&o)
    : m_Format(o.m_Format),
      m_Swapchain(o.m_Swapchain),
      m_Extent(o.m_Extent),
      m_Images(std::move(o.m_Images)),
      m_ImageViews(std::move(o.m_ImageViews)) {
  o.m_Swapchain = nullptr;
  o.m_Extent = {0, 0};
  o.m_Images.clear();
  o.m_ImageViews.clear();
}

VulkanSwapchain &VulkanSwapchain::operator=(VulkanSwapchain &&o) {
  m_Format = o.m_Format;
  m_Swapchain = o.m_Swapchain;
  m_Extent = o.m_Extent;
  m_Images = std::move(o.m_Images);
  m_ImageViews = std::move(o.m_ImageViews);
  o.m_Swapchain = nullptr;
  o.m_Extent = {0, 0};
  o.m_Images.clear();
  o.m_ImageViews.clear();

  return *this;
}

VulkanSwapchain::~VulkanSwapchain() {
  VulkanContext &vctx = VulkanContext::get();
  for (auto imageView : m_ImageViews) {
    vkDestroyImageView(vctx.device(), imageView, vctx.allocator());
    FROTH_DEBUG("Destroyed Vulkan Swapchain Image View")
  }

  if (m_Swapchain) {
    vkDestroySwapchainKHR(vctx.device(), m_Swapchain, vctx.allocator());
    m_Swapchain = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Swapchain");
  }
}

VkSurfaceFormatKHR VulkanSwapchain::chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
  // TODO: Improve the selection algorithm
  // This is very rudimentary right now
  for (const auto &format : availableFormats) {
    if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return format;
    }
  }
  return availableFormats[0];
}

VkPresentModeKHR VulkanSwapchain::choosePresentMode(const std::vector<VkPresentModeKHR> &availableModes) {
  // TODO: Improve the selection algorithm
  for (const auto &mode : availableModes) {
    if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
      return mode;
  }

  return VK_PRESENT_MODE_FIFO_KHR; // Guaranteed to be available
}

VkExtent2D VulkanSwapchain::chooseExtent(const VulkanSurface &surface, const VulkanDevice::SurfaceCapabilities &capabilities) {
  if (capabilities.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capabilities.capabilities.currentExtent;
  }

  VkExtent2D actualExtent = surface.extent();

  actualExtent.width = std::clamp(actualExtent.width, capabilities.capabilities.minImageExtent.width, capabilities.capabilities.maxImageExtent.width);
  actualExtent.height = std::clamp(actualExtent.height, capabilities.capabilities.minImageExtent.height, capabilities.capabilities.maxImageExtent.height);
  return actualExtent;
}

} // namespace Froth
