#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "src/core/events/ApplicationEvent.h"
#include "src/platform/window/Window.h"
#include "src/renderer/Renderer.h"
#include "src/renderer/vulkan/VulkanCommandBuffer.h"
#include "src/renderer/vulkan/VulkanCommandPool.h"
#include "src/renderer/vulkan/VulkanContext.h"
#include "src/renderer/vulkan/VulkanDescriptorPool.h"
#include "src/renderer/vulkan/VulkanDescriptorSetLayout.h"
#include "src/renderer/vulkan/VulkanDevice.h"
#include "src/renderer/vulkan/VulkanImage.h"
#include "src/renderer/vulkan/VulkanIndexBuffer.h"
#include "src/renderer/vulkan/VulkanPipeline.h"
#include "src/renderer/vulkan/VulkanPipelineLayout.h"
#include "src/renderer/vulkan/VulkanSurface.h"
#include "src/renderer/vulkan/VulkanSwapchainManager.h"
#include "src/renderer/vulkan/VulkanVertexBuffer.h"
#include "src/resources/materials/Material.h"
#include <memory>

namespace Froth {
class VulkanRenderer : public Renderer {
  friend class Renderer;

public:
  VulkanRenderer() = default;
  VulkanRenderer(const Window &window);
  static void shutdown();

  ~VulkanRenderer() override;
  VulkanRenderer(VulkanRenderer const &) = delete;
  void operator=(VulkanRenderer const &) = delete;
  VulkanRenderer(VulkanRenderer &&);
  VulkanRenderer &operator=(VulkanRenderer &&);

  virtual bool onEvent(const Event &e) override;
  bool onFramebufferResize(FramebufferResizeEvent &e);

  virtual bool beginFrame() override;
  virtual void beginRenderPass() override;
  virtual void endRenderPass() override;
  virtual void endFrame() override;

  virtual void pushConstants(const glm::mat4 &data) const override;
  virtual void bindMaterial(const Material &mat) override;

  VulkanCommandPool &getCurrentCommandPool();
  VulkanCommandPool &getGraphicsCommandPool() { return m_GraphicsCommandPool; };
  VulkanDescriptorPool &getDescriptorPool() { return m_DescriptorPool; }

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
  VulkanDescriptorPool m_DescriptorPool;
  std::unique_ptr<VulkanPipelineLayout> m_PipelineLayout = nullptr;
  std::unique_ptr<VulkanPipeline> m_Pipeline = nullptr;

  std::unique_ptr<VulkanPipeline> buildPipeline(const Material &mat);
};
} // namespace Froth
