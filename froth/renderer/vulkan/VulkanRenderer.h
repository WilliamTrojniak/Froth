#pragma once

#include "core/events/ApplicationEvent.h"
#include "platform/window/Window.h"
#include "renderer/Renderer.h"
#include "renderer/vulkan/VulkanCommandBuffer.h"
#include "renderer/vulkan/VulkanCommandPool.h"
#include "renderer/vulkan/VulkanDescriptorSetLayout.h"
#include "renderer/vulkan/VulkanDevice.h"
#include "renderer/vulkan/VulkanFence.h"
#include "renderer/vulkan/VulkanFramebuffer.h"
#include "renderer/vulkan/VulkanImage.h"
#include "renderer/vulkan/VulkanImageView.h"
#include "renderer/vulkan/VulkanIndexBuffer.h"
#include "renderer/vulkan/VulkanInstance.h"
#include "renderer/vulkan/VulkanPipeline.h"
#include "renderer/vulkan/VulkanPipelineLayout.h"
#include "renderer/vulkan/VulkanRenderPass.h"
#include "renderer/vulkan/VulkanSemaphore.h"
#include "renderer/vulkan/VulkanSurface.h"
#include "renderer/vulkan/VulkanSwapchain.h"
#include "renderer/vulkan/VulkanVertexBuffer.h"
#include <memory>
#include <vector>

namespace Froth {
class VulkanRenderer : public Renderer {
  friend class Renderer;

public:
  virtual void onUpdate(double ts) override;

  ~VulkanRenderer() override;
  VulkanRenderer(VulkanRenderer const &) = delete;
  void operator=(VulkanRenderer const &) = delete;

  virtual bool onEvent(const Event &e) override;
  bool onWindowResize(WindowResizeEvent &e);

  virtual bool beginFrame() override;
  virtual void beginRenderPass() override;
  virtual void endRenderPass() override;
  virtual void endFrame() override;

  virtual std::unique_ptr<VertexBuffer> createVertexBuffer(size_t sizeBytes) override;

  void bindVertexBuffer(const VulkanVertexBuffer &buffer) const;

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
  VulkanDescriptorSetLayout m_DescriptorSetLayout;
  VulkanCommandPool m_GraphicsCommandPool;
  std::unique_ptr<VulkanSwapChain> m_Swapchain;
  std::unique_ptr<VulkanImage> m_DepthImage;
  std::unique_ptr<VulkanImageView> m_DepthImageView;
  std::unique_ptr<VulkanRenderPass> m_RenderPass;
  std::unique_ptr<VulkanPipelineLayout> m_PipelineLayout;
  std::unique_ptr<VulkanPipeline> m_Pipeline;
  std::vector<VulkanFramebuffer> m_Framebuffers;
  std::vector<VulkanCommandBuffer> m_CommandBuffers;
  std::vector<VulkanSemaphore> m_ImageAvailableSemaphores;
  std::vector<VulkanSemaphore> m_RenderFinishedSemaphores;
  std::vector<VulkanFence> m_FrameInFlightFences;
  std::unique_ptr<VulkanIndexBuffer> m_IndexBuffer;
  size_t m_CurrentFrame = 0;
  uint32_t m_CurrentImageIndex = 0;
  bool m_WindowResized = false;

  void shutdown() noexcept;

  void recreateSwapchain();
};
} // namespace Froth
