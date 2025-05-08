#pragma once

#include "VulkanDevice.h"

namespace Froth {

class VulkanCommandBuffer {
  friend class VulkanCommandPool;

public:
  ~VulkanCommandBuffer();

  VulkanCommandBuffer(VulkanCommandBuffer const &) = delete;
  VulkanCommandBuffer &operator=(VulkanCommandBuffer const &) = delete;

  VulkanCommandBuffer(VulkanCommandBuffer &&);
  operator VkCommandBuffer() const { return m_Buffer; }
  void cleanup(VkCommandPool pool);

protected:
  VulkanCommandBuffer() = default;
  VkCommandBuffer &data() { return m_Buffer; }

private:
  VkCommandBuffer m_Buffer = nullptr;
};

} // namespace Froth
