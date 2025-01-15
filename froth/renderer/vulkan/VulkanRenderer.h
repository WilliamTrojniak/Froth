#pragma once

#include "renderer/Renderer.h"
#include "renderer/vulkan/VulkanDevice.h"
#include "renderer/vulkan/VulkanInstance.h"
#include "renderer/vulkan/VulkanSurface.h"
#include <memory>

namespace Froth {
class VulkanRenderer : public Renderer {
  friend class Renderer;

public:
  ~VulkanRenderer() override;
  VulkanRenderer(VulkanRenderer const &) = delete;
  void operator=(VulkanRenderer const &) = delete;

  virtual bool init(const Window &window) noexcept override;

protected:
  VulkanRenderer();

private:
  static bool s_Initialized;
  static VulkanInstance s_Ctx;
  std::unique_ptr<VulkanSurface> m_Surface; // HACK: Can this be stack-allocated somehow?
  std::unique_ptr<VulkanDevice> m_Device;   // HACK: Can this be stack-allocated somehow?

  void shutdown() noexcept;
};
} // namespace Froth
