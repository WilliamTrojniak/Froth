#include "VulkanDevice.h"
#include <core/logger/Logger.h>
#include <set>
#include <string>
#include <vector>

namespace Froth {

VulkanDevice::VulkanDevice(const VulkanInstance &instance, const VulkanSurface &surface) : m_Instance(instance) {
  // TODO: This should be driven by the Engine
  // Device Requirements
  PhysicalDeviceProperties requirements{};
  requirements.graphics = true;
  requirements.present = true;
  requirements.compute = false;
  requirements.transfer = true;

  requirements.extensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
  requirements.extensions.emplace_back("VK_KHR_portability_subset"); // TODO: Only enable on MacOS

#ifdef FROTH_BUILD_DEBUG
  requirements.layers.emplace_back("VK_LAYER_KHRONOS_validation");
#endif // FROTH_BUILD_DEBUG

  requirements.layers.emplace_back("VK_LAYER_KHRONOS_validation");
  requirements.samplerAnisotropy = true; // TODO: This can probably be made optional?
  // Device Requirements

  m_PhysicalDevice = pickPhysicalDevice(m_Instance.instance(), surface, requirements);
  if (m_PhysicalDevice == nullptr) {
    FROTH_ERROR("Failed to find suitable Vulkan physical device")
  }

  m_QueueFamilies = getPhysicalDeviceQueueFamilies(m_PhysicalDevice, surface);
  m_LogicalDevice = createLogicalDevice(m_Instance, m_PhysicalDevice, m_QueueFamilies, requirements);
  if (m_LogicalDevice == nullptr) {
    FROTH_ERROR("Failed to create Vulkan Logical Device from physical device")
  }
}

VulkanDevice::~VulkanDevice() {
  if (m_LogicalDevice != nullptr) {
    vkDestroyDevice(m_LogicalDevice, m_Instance.allocator());
    m_LogicalDevice = nullptr;
    FROTH_DEBUG("Destroyed Vulkan logical device")
  }
}

VulkanDevice::SurfaceCapabilities VulkanDevice::getSurfaceSupport(const VulkanSurface &surface) const {
  return VulkanDevice::physicalDeviceSurfaceSupport(m_PhysicalDevice, surface);
}

VkDeviceMemory VulkanDevice::allocateMemory(const VkMemoryRequirements &requirements, VkMemoryPropertyFlags properties) const {
  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = requirements.size;
  allocInfo.memoryTypeIndex = findMemoryTypeIndex(requirements, properties);

  VkDeviceMemory memory;
  if (vkAllocateMemory(m_LogicalDevice, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
    FROTH_ERROR("Failed to allocate device memory");
    throw std::runtime_error("failed to allocate buffer memory");
  }
  return memory;
}

uint32_t VulkanDevice::findMemoryTypeIndex(const VkMemoryRequirements &requirements, VkMemoryPropertyFlags properties) const {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if (requirements.memoryTypeBits & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }
  FROTH_ERROR("Failed to find suitable memory type");
  return -1;
}

VkPhysicalDevice VulkanDevice::pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, const PhysicalDeviceProperties &requirements) noexcept {
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

  if (!VulkanDevice::physicalDeviceSupportsLayers(device, requirements.layers)) {
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

const VulkanDevice::QueueFamilies VulkanDevice::getQueueFamilies() const noexcept {
  return m_QueueFamilies;
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

bool VulkanDevice::physicalDeviceSupportsLayers(VkPhysicalDevice device, const std::vector<const char *> &layers) noexcept {
  uint32_t availableLayersCount;
  if (vkEnumerateDeviceLayerProperties(device, &availableLayersCount, nullptr) != VK_SUCCESS) {
    return false;
  }

  std::vector<VkLayerProperties> availableLayers(availableLayersCount);
  if (vkEnumerateDeviceLayerProperties(device, &availableLayersCount, availableLayers.data()) != VK_SUCCESS) {
    return false;
  }

  std::set<std::string> layerSet(layers.begin(), layers.end());
  for (const auto &l : availableLayers) {
    layerSet.erase(l.layerName);
  }

  return layerSet.empty();
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

VkDevice VulkanDevice::createLogicalDevice(const VulkanInstance &context, VkPhysicalDevice physicalDevice, const QueueFamilies &queueFamilies, const PhysicalDeviceProperties &requirements) noexcept {
  std::set<uint32_t> uniqueQueueFamilyIndices;
  if (queueFamilies.graphics.valid) {
    uniqueQueueFamilyIndices.emplace(queueFamilies.graphics.index);
  }
  if (queueFamilies.present.valid) {
    uniqueQueueFamilyIndices.emplace(queueFamilies.present.index);
  }
  if (queueFamilies.compute.valid) {
    uniqueQueueFamilyIndices.emplace(queueFamilies.compute.index);
  }
  if (queueFamilies.transfer.valid) {
    uniqueQueueFamilyIndices.emplace(queueFamilies.transfer.index);
  }

  float queuePriority = 1.0f; // TODO: make dynamic
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  for (const auto &index : uniqueQueueFamilyIndices) {
    VkDeviceQueueCreateInfo queueCreateInfo{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
    queueCreateInfo.queueFamilyIndex = index;
    queueCreateInfo.queueCount = 1; // TODO: Dynamic
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = requirements.samplerAnisotropy;

  VkDeviceCreateInfo deviceCreateInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
  deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
  deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
  deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
  deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requirements.extensions.size());
  deviceCreateInfo.ppEnabledExtensionNames = requirements.extensions.data();
  deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(requirements.layers.size());
  deviceCreateInfo.ppEnabledLayerNames = requirements.layers.data();

  VkDevice device;
  if (vkCreateDevice(physicalDevice, &deviceCreateInfo, context.allocator(), &device) != VK_SUCCESS) {
    return nullptr;
  }

  return device;
}

} // namespace Froth
