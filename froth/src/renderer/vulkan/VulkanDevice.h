#pragma once

#include "VulkanSurface.h"
#include <vector>

namespace Froth {
class VulkanDevice {
  friend class VulkanContext;

public:
  VulkanDevice(VulkanDevice const &) = delete;
  VulkanDevice &operator=(VulkanDevice const &) = delete;

  VulkanDevice(VulkanDevice &&);
  VulkanDevice &operator=(VulkanDevice &&);

  ~VulkanDevice();
  void cleanup();

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

  const QueueFamilies getQueueFamilies() const noexcept;
  static SurfaceCapabilities physicalDeviceSurfaceSupport(VkPhysicalDevice device, VkSurfaceKHR surface) noexcept;

protected:
  VulkanDevice() = default;
  VulkanDevice(const VkAllocationCallbacks *allocator, VkPhysicalDevice physicalDevice, const QueueFamilies &queueFamilies, const PhysicalDeviceProperties &requirements);

private:
  VkDevice m_LogicalDevice = nullptr;
  QueueFamilies m_QueueFamilies;

  uint32_t findMemoryTypeIndex(const VkMemoryRequirements &requirements, VkMemoryPropertyFlags properties) const;

  static VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, const PhysicalDeviceProperties &requirements) noexcept;
  static uint32_t ratePhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR surface) noexcept;
  static bool physicalDeviceMeetsRequirements(VkPhysicalDevice device, VkSurfaceKHR surface, const PhysicalDeviceProperties &requirements) noexcept;
  static QueueFamilies getPhysicalDeviceQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) noexcept;
  static bool physicalDeviceSupportsExtensions(VkPhysicalDevice device, const std::vector<const char *> &extensions) noexcept;
  static bool physicalDeviceSupportsLayers(VkPhysicalDevice device, const std::vector<const char *> &layers) noexcept;
};

} // namespace Froth
