#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Froth {
class VulkanDevice {
  friend class VulkanRenderer;

public:
  static bool create(VkInstance instance, VkSurfaceKHR surface, VulkanDevice &device) noexcept;

  struct PhysicalDeviceProperties {
    bool graphics;
    bool present;
    bool compute;
    bool transfer;

    std::vector<const char *> extensions;
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

protected:
  VulkanDevice() = default;
  VulkanDevice(VkPhysicalDevice physicalDevice, VkDevice logicalDevice);

private:
  VkPhysicalDevice m_PhysicalDevice;
  VkDevice m_LogicalDevice;
  QueueFamilies m_Queues;

  static VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) noexcept;
  static uint32_t ratePhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR surface) noexcept;
  static bool physicalDeviceMeetsRequirements(VkPhysicalDevice device, VkSurfaceKHR surface, const PhysicalDeviceProperties &requirements) noexcept;
  static QueueFamilies getPhysicalDeviceQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) noexcept;
  static bool physicalDeviceSupportsExtensions(VkPhysicalDevice device, const std::vector<const char *> &extensions) noexcept;
  static SurfaceCapabilities physicalDeviceSurfaceSupport(VkPhysicalDevice device, VkSurfaceKHR surface) noexcept;
};

} // namespace Froth
