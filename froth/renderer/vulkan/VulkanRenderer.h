#pragma once

#include "renderer/Renderer.h"
#include "renderer/vulkan/VulkanCommandBuffer.h"
#include "renderer/vulkan/VulkanCommandPool.h"
#include "renderer/vulkan/VulkanDescriptorSetLayout.h"
#include "renderer/vulkan/VulkanDevice.h"
#include "renderer/vulkan/VulkanFramebuffer.h"
#include "renderer/vulkan/VulkanImage.h"
#include "renderer/vulkan/VulkanImageView.h"
#include "renderer/vulkan/VulkanInstance.h"
#include "renderer/vulkan/VulkanPipeline.h"
#include "renderer/vulkan/VulkanPipelineLayout.h"
#include "renderer/vulkan/VulkanRenderPass.h"
#include "renderer/vulkan/VulkanSurface.h"
#include "renderer/vulkan/VulkanSwapchain.h"
#include <memory>
#include <vector>

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
  VulkanImageView m_DepthImageView;
  VulkanRenderPass m_RenderPass;
  VulkanDescriptorSetLayout m_DescriptorSetLayout;
  VulkanPipelineLayout m_PipelineLayout;
  std::unique_ptr<VulkanPipeline> m_Pipeline;
  std::vector<VulkanFramebuffer> m_Framebuffers;
  VulkanCommandPool m_GraphicsCommandPool;
  std::vector<VulkanCommandBuffer> m_CommandBuffers;

  void shutdown() noexcept;
};
} // namespace Froth
