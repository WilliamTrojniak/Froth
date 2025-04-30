#pragma once

#include "platform/window/Window.h"
#include "renderer/IndexBuffer.h"
#include "renderer/VertexBuffer.h"
#include <memory>

namespace Froth {

class Renderer {
protected:
  Renderer() {};

public:
  virtual ~Renderer() = default;

  virtual void shutdown() = 0;
  virtual bool onEvent(const Event &e) = 0;

  virtual bool beginFrame() = 0;
  virtual void beginRenderPass() = 0;
  virtual void endRenderPass() = 0;
  virtual void endFrame() = 0;

  virtual std::unique_ptr<VertexBuffer> createVertexBuffer(size_t sizeBytes) = 0;
  virtual std::unique_ptr<IndexBuffer> createIndexBuffer(size_t numIndices) = 0;

  /* Creates an abstract Renderer backend
   *
   * @returns  Renderer Backend
   * @throws std::runtime_error if Window Surface cannot be created
   */
  static std::unique_ptr<Renderer> create(const Window &window);
};

} // namespace Froth
