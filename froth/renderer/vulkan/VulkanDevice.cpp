#include "VulkanDevice.h"
#include <core/logger/Logger.h>
#include <set>
#include <string>

namespace Froth {

VulkanDevice::VulkanDevice(VkPhysicalDevice physicalDevice, VkDevice device) : m_PhysicalDevice(physicalDevice), m_LogicalDevice(device) {
}

bool VulkanDevice::create(VkInstance instance, VkSurfaceKHR surface, VulkanDevice &device) noexcept {
  VkPhysicalDevice physicalDevice = pickPhysicalDevice(instance, surface);
  if (physicalDevice == nullptr) {
    FROTH_WARN("Failed to find suitable Vulkan physical device")
    return false;
  }

  VkDevice logicalDevice;

  device = VulkanDevice(physicalDevice, logicalDevice);
  return true;
}

VkPhysicalDevice VulkanDevice::pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) noexcept {
  uint32_t physicalDeviceCount;
  if (vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr) != VK_SUCCESS || physicalDeviceCount == 0) {
    FROTH_WARN("No physical devices with Vulkan Support were detected.")
    return nullptr;
  };
  std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
  if (vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()) != VK_SUCCESS) {
    FROTH_WARN("Failed to retreive available Vulkan physical devices")
    return nullptr;
  }

  // TODO: This should be driven by the Engine
  // Device Requirements
  PhysicalDeviceProperties requirements{};
  requirements.graphics = true;
  requirements.present = surface != nullptr;
  requirements.compute = false;
  requirements.transfer = true;

  if (surface != nullptr) {
    requirements.extensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
  }

  requirements.extensions.emplace_back("VK_KHR_portability_subset"); // TODO: Only enable on MacOS
  requirements.samplerAnisotropy = true;                             // TODO: This can probably be made optional?
  // Device Requirements

  // Select Best Available Physical Device
  uint32_t maxScore = 0;
  VkPhysicalDevice selectedDevice = nullptr;
  for (const auto &device : physicalDevices) {
    if (VulkanDevice::physicalDeviceMeetsRequirements(device, surface, requirements)) {
      uint32_t score = VulkanDevice::ratePhysicalDevice(device, surface);
      if (selectedDevice == nullptr || score > maxScore) {
        selectedDevice = device;
        maxScore = score;
      }
    }
  }

  return selectedDevice;
}

uint32_t VulkanDevice::ratePhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR surface) noexcept {
  const uint32_t TYPE_DISCRETE_SCORE = 4;
  const uint32_t TYPE_INTEGRATED_SCORE = 2;
  uint32_t score = 0;

  VkPhysicalDeviceProperties deviceProperties;
  VkPhysicalDeviceFeatures deviceFeatures;
  VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
  vkGetPhysicalDeviceMemoryProperties(device, &deviceMemoryProperties);

  // Score physical device type
  switch (deviceProperties.deviceType) {
  case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
    score += TYPE_DISCRETE_SCORE;
    break;
  case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
    score += TYPE_INTEGRATED_SCORE;
    break;
  default:
    break;
  }

  // TODO: Score other features as well

  return score;
}

bool VulkanDevice::physicalDeviceMeetsRequirements(VkPhysicalDevice device, VkSurfaceKHR surface, const PhysicalDeviceProperties &requirements) noexcept {
  VkPhysicalDeviceProperties deviceProperties;
  VkPhysicalDeviceFeatures deviceFeatures;
  VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
  vkGetPhysicalDeviceMemoryProperties(device, &deviceMemoryProperties);

  if (requirements.samplerAnisotropy && !deviceFeatures.samplerAnisotropy) {
    return false;
  }

  QueueFamilies queueFamilies = getPhysicalDeviceQueueFamilies(device, surface);
  if ((requirements.graphics && !queueFamilies.graphics.valid) ||
      (requirements.present && !queueFamilies.present.valid) ||
      (requirements.compute && !queueFamilies.compute.valid) ||
      (requirements.transfer && !queueFamilies.transfer.valid)) {
    return false;
  }

  if (!VulkanDevice::physicalDeviceSupportsExtensions(device, requirements.extensions)) {
    return false;
  }

  // Only query surface support after checking for extensions
  SurfaceCapabilities surfaceCapabilities = VulkanDevice::physicalDeviceSurfaceSupport(device, surface);
  if (requirements.present &&
      (surfaceCapabilities.presentModes.empty() || surfaceCapabilities.formats.empty())) {
    return false;
  }

  return true;
}

VulkanDevice::QueueFamilies VulkanDevice::getPhysicalDeviceQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) noexcept {
  uint32_t deviceQueueFamilyCount;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &deviceQueueFamilyCount, nullptr);
  std::vector<VkQueueFamilyProperties> deviceQueueFamilyProperties(deviceQueueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &deviceQueueFamilyCount, deviceQueueFamilyProperties.data());

  QueueFamilies queueFamilies{};
  uint32_t minTransferScore = -1;
  for (int32_t i = 0; i < deviceQueueFamilyProperties.size(); i++) {
    VkQueueFamilyProperties queueFamilyProperties = deviceQueueFamilyProperties[i];
    uint32_t currentTransferScore = 0;

    if (queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT && !queueFamilies.graphics.valid) {
      queueFamilies.graphics.valid = true;
      queueFamilies.graphics.index = i;
      currentTransferScore++;
    }

    if (queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) {
      if (!queueFamilies.compute.valid ||
          queueFamilies.graphics.index == queueFamilies.compute.index) {
        queueFamilies.compute.index = i;
        currentTransferScore++;
      }
      queueFamilies.compute.valid = true;
    }

    VkBool32 supportsPresentation;
    if (vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &supportsPresentation) != VK_SUCCESS) {
      supportsPresentation = false;
    }

    if (supportsPresentation) {
      if (!queueFamilies.present.valid ||
          (queueFamilies.graphics.index == queueFamilies.present.index) ||
          (queueFamilies.compute.index == queueFamilies.present.index)) {
        queueFamilies.present.index = i;
        currentTransferScore++;
      }
      queueFamilies.present.valid = true;
    }

    if (queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT &&
        (!queueFamilies.transfer.valid ||
         currentTransferScore < minTransferScore)) {
      queueFamilies.transfer.valid = true;
      queueFamilies.transfer.index = i;
      minTransferScore = currentTransferScore;
    }
  }
  return queueFamilies;
}

bool VulkanDevice::physicalDeviceSupportsExtensions(VkPhysicalDevice device, const std::vector<const char *> &extensions) noexcept {
  uint32_t availableExtensionsCount;
  if (vkEnumerateDeviceExtensionProperties(device, nullptr, &availableExtensionsCount, nullptr) != VK_SUCCESS) {
    return false;
  }

  std::vector<VkExtensionProperties> availableExtensions(availableExtensionsCount);
  if (vkEnumerateDeviceExtensionProperties(device, nullptr, &availableExtensionsCount, availableExtensions.data()) != VK_SUCCESS) {
    return false;
  }

  std::set<std::string> extensionsSet(extensions.begin(), extensions.end());
  for (const VkExtensionProperties &e : availableExtensions) {
    extensionsSet.erase(e.extensionName);
  }

  return extensionsSet.empty();
}

VulkanDevice::SurfaceCapabilities VulkanDevice::physicalDeviceSurfaceSupport(VkPhysicalDevice device, VkSurfaceKHR surface) noexcept {
  SurfaceCapabilities capabilities;

  if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities.capabilities) != VK_SUCCESS) {
    capabilities.capabilities = VkSurfaceCapabilitiesKHR{};
  }

  uint32_t formatCount;
  if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr) != VK_SUCCESS) {
    formatCount = 0;
  }
  capabilities.formats.resize(formatCount);
  if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, capabilities.formats.data()) != VK_SUCCESS) {
    capabilities.formats.resize(0);
  }

  uint32_t presentModeCount;
  if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr) != VK_SUCCESS) {
    presentModeCount = 0;
  }
  capabilities.presentModes.resize(presentModeCount);
  if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, capabilities.presentModes.data()) != VK_SUCCESS) {
    capabilities.presentModes.resize(0);
  }

  return capabilities;
}

} // namespace Froth
