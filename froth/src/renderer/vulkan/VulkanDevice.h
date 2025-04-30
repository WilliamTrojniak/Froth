#pragma once

#include "src/renderer/vulkan/VulkanInstance.h"
#include "src/renderer/vulkan/VulkanSurface.h"
#include <vector>

namespace Froth {
class VulkanDevice {
  friend class VulkanRenderer;

public:
  VulkanDevice() = default;
  VulkanDevice(const VulkanSurface &surface);

  VulkanDevice(VulkanDevice const &) = delete;
  VulkanDevice &operator=(VulkanDevice const &) = delete;

  VulkanDevice(VulkanDevice &&);
  VulkanDevice &operator=(VulkanDevice &&);

  ~VulkanDevice();

  operator VkDevice() const noexcept { return m_LogicalDevice; }
  VkDeviceMemory allocateMemory(const VkMemoryRequirements &requirements, VkMemoryPropertyFlags properties) const;

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
    VkQueue queue;
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

  SurfaceCapabilities getSurfaceSupport(const VulkanSurface &surface) const;
  const QueueFamilies getQueueFamilies() const noexcept;

private:
  VkPhysicalDevice m_PhysicalDevice;
  VkDevice m_LogicalDevice = nullptr;
  QueueFamilies m_QueueFamilies;

  uint32_t findMemoryTypeIndex(const VkMemoryRequirements &requirements, VkMemoryPropertyFlags properties) const;

  static VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, const PhysicalDeviceProperties &requirements) noexcept;
  static uint32_t ratePhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR surface) noexcept;
  static bool physicalDeviceMeetsRequirements(VkPhysicalDevice device, VkSurfaceKHR surface, const PhysicalDeviceProperties &requirements) noexcept;
  static QueueFamilies getPhysicalDeviceQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) noexcept;
  static bool physicalDeviceSupportsExtensions(VkPhysicalDevice device, const std::vector<const char *> &extensions) noexcept;
  static bool physicalDeviceSupportsLayers(VkPhysicalDevice device, const std::vector<const char *> &layers) noexcept;
  static SurfaceCapabilities physicalDeviceSurfaceSupport(VkPhysicalDevice device, VkSurfaceKHR surface) noexcept;

  static VkDevice createLogicalDevice(const VulkanInstance &context, VkPhysicalDevice physicalDevice, QueueFamilies &queueFamilies, const PhysicalDeviceProperties &requirements) noexcept;
};

} // namespace Froth
