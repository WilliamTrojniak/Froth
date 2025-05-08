#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "src/core/events/ApplicationEvent.h"
#include "src/platform/window/Window.h"
#include "src/renderer/Renderer.h"
#include "src/renderer/vulkan/VulkanCommandBuffer.h"
#include "src/renderer/vulkan/VulkanCommandPool.h"
#include "src/renderer/vulkan/VulkanContext.h"
#include "src/renderer/vulkan/VulkanDescriptorSetLayout.h"
#include "src/renderer/vulkan/VulkanDevice.h"
#include "src/renderer/vulkan/VulkanFence.h"
#include "src/renderer/vulkan/VulkanImage.h"
#include "src/renderer/vulkan/VulkanIndexBuffer.h"
#include "src/renderer/vulkan/VulkanPipeline.h"
#include "src/renderer/vulkan/VulkanPipelineLayout.h"
#include "src/renderer/vulkan/VulkanSemaphore.h"
#include "src/renderer/vulkan/VulkanSurface.h"
#include "src/renderer/vulkan/VulkanSwapchainManager.h"
#include "src/renderer/vulkan/VulkanVertexBuffer.h"
#include "src/resources/materials/Material.h"
#include <memory>
#include <vector>

namespace Froth {
class VulkanRenderer : public Renderer {
  friend class Renderer;
  struct _tag {
    explicit _tag() = default;
  };

public:
  VulkanRenderer(const Window &window, _tag);
  static void shutdown();

  ~VulkanRenderer() override;
  VulkanRenderer(VulkanRenderer const &) = delete;
  void operator=(VulkanRenderer const &) = delete;

  virtual bool onEvent(const Event &e) override;
  bool onFramebufferResize(FramebufferResizeEvent &e);

  virtual bool beginFrame() override;
  virtual void beginRenderPass() override;
  virtual void endRenderPass() override;
  virtual void endFrame() override;

  virtual std::unique_ptr<VertexBuffer> createVertexBuffer(size_t sizeBytes) override;
  virtual std::unique_ptr<IndexBuffer> createIndexBuffer(size_t sizeBytes) override;

  virtual void pushConstants(const glm::mat4 &data) const override;
  virtual void bindMaterial(const Material &mat) override;

  void bindVertexBuffer(const VulkanVertexBuffer &buffer) const;
  void bindIndexBuffer(const VulkanIndexBuffer &buffer) const;

protected:
  /* Creates a Vulkan Renderer backend
   *
   * @returns Vulkan Renderer Backend
   * @throws std::runtime_error if Window Surface cannot be created
   */
  static std::unique_ptr<VulkanRenderer> create(const Window &window);
  static void init(const Window &window) { VulkanContext::get().init(window); };

private:
  VulkanDescriptorSetLayout m_DescriptorSetLayout;
  VulkanCommandPool m_GraphicsCommandPool;
  VulkanSwapchainManager m_SwapchainManager;
  std::unique_ptr<VulkanPipelineLayout> m_PipelineLayout;
  std::unique_ptr<VulkanPipeline> m_Pipeline;
  std::vector<VulkanCommandBuffer> m_CommandBuffers;
  std::vector<VulkanSemaphore> m_ImageAvailableSemaphores;
  std::vector<VulkanSemaphore> m_RenderFinishedSemaphores;
  std::vector<VulkanFence> m_FrameInFlightFences;
  uint32_t m_CurrentFrame = 0;
  uint32_t m_CurrentImageIndex = 0;
  bool m_WindowResized = false;
  Material m_Mat;

  std::unique_ptr<VulkanPipeline> buildPipeline(const Material &mat);
};
} // namespace Froth
