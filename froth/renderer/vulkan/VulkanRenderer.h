#pragma once

#include "renderer/Renderer.h"
#include "renderer/vulkan/VulkanDevice.h"
#include "renderer/vulkan/VulkanImage.h"
#include "renderer/vulkan/VulkanInstance.h"
#include "renderer/vulkan/VulkanSurface.h"
#include "renderer/vulkan/VulkanSwapchain.h"
#include <memory>

namespace Froth {
class VulkanRenderer : public Renderer {
  friend class Renderer;

public:
  ~VulkanRenderer() override;
  VulkanRenderer(VulkanRenderer const &) = delete;
  void operator=(VulkanRenderer const &) = delete;

protected:
  VulkanRenderer(const Window &window);

  /* Creates a Vulkan Renderer backend
   *
   * @returns Vulkan Renderer Backend
   * @throws std::runtime_error if Window Surface cannot be created
   */
  static std::unique_ptr<VulkanRenderer> create(const Window &window);

private:
  static bool s_Initialized;
  static VulkanInstance s_Ctx;
  VulkanSurface m_Surface;
  VulkanDevice m_Device;
  VulkanSwapChain m_Swapchain;
  VulkanImage m_DepthImage;

  void shutdown() noexcept;
};
} // namespace Froth
