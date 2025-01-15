#pragma once

#include "renderer/vulkan/VulkanInstance.h"
#include "renderer/vulkan/VulkanSurface.h"
#include <vector>

namespace Froth {
class VulkanDevice {
  friend class VulkanRenderer;

public:
  VulkanDevice(const VulkanInstance &instance, const VulkanSurface &surface);
  VulkanDevice(VulkanDevice const &) = delete;
  void operator=(VulkanDevice const &) = delete;
  ~VulkanDevice();

  struct PhysicalDeviceProperties {
    bool graphics;
    bool present;
    bool compute;
    bool transfer;

    std::vector<const char *> extensions;
    std::vector<const char *> layers;
    bool samplerAnisotropy;
  };

  struct VulkanQueueFamilyInfo {
    bool valid;
    uint32_t index;
  };

  struct QueueFamilies {
    VulkanQueueFamilyInfo graphics;
    VulkanQueueFamilyInfo present;
    VulkanQueueFamilyInfo compute;
    VulkanQueueFamilyInfo transfer;
  };

  struct SurfaceCapabilities {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

private:
  VkPhysicalDevice m_PhysicalDevice;
  VkDevice m_LogicalDevice = nullptr;
  const VulkanInstance &m_Instance;
  QueueFamilies m_Queues;

  static VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, const PhysicalDeviceProperties &requirements) noexcept;
  static uint32_t ratePhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR surface) noexcept;
  static bool physicalDeviceMeetsRequirements(VkPhysicalDevice device, VkSurfaceKHR surface, const PhysicalDeviceProperties &requirements) noexcept;
  static QueueFamilies getPhysicalDeviceQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) noexcept;
  static bool physicalDeviceSupportsExtensions(VkPhysicalDevice device, const std::vector<const char *> &extensions) noexcept;
  static bool physicalDeviceSupportsLayers(VkPhysicalDevice device, const std::vector<const char *> &layers) noexcept;
  static SurfaceCapabilities physicalDeviceSurfaceSupport(VkPhysicalDevice device, VkSurfaceKHR surface) noexcept;

  static VkDevice createLogicalDevice(const VulkanInstance &context, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const PhysicalDeviceProperties &requirements) noexcept;
};

} // namespace Froth
