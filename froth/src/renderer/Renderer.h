#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "src/platform/window/Window.h"
#include "src/resources/materials/Material.h"
#include <memory>

namespace Froth {

class Renderer {
protected:
  Renderer() {};

public:
  virtual ~Renderer() = default;

  virtual bool onEvent(const Event &e) = 0;

  virtual bool beginFrame() = 0;
  virtual void beginRenderPass() = 0;
  virtual void endRenderPass() = 0;
  virtual void endFrame() = 0;

  virtual void pushConstants(const glm::mat4 &data) const = 0;
  virtual void bindMaterial(const Material &mat) = 0;

  /* Creates an abstract Renderer backend
   *
   * @returns  Renderer Backend
   * @throws std::runtime_error if Window Surface cannot be created
   */
  static std::unique_ptr<Renderer> create(const Window &window);
  static void init(const Window &window);
  static void shutdown();
};

} // namespace Froth
