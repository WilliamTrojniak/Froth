
#include "renderer/vulkan/VulkanDevice.h"
#include "renderer/vulkan/VulkanImageView.h"
#include "renderer/vulkan/VulkanSwapchain.h"
#include "vulkan/vulkan_core.h"
namespace Froth {
class VulkanRenderPass {
  friend class VulkanRenderer;

public:
  ~VulkanRenderPass();
  VulkanRenderPass(VulkanRenderPass const &) = delete;
  void operator=(VulkanImageView const &) = delete;

protected:
  VulkanRenderPass(const VulkanDevice &device, const VulkanSwapChain &swapchain, const VulkanImageView &depthImageView);

private:
  const VulkanDevice &m_Device;
  VkRenderPass m_RenderPass;

  void cleanup();
};
} // namespace Froth
