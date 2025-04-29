#pragma once

#include "platform/window/Window.h"
#include <memory>
namespace Froth {

class Renderer {
protected:
  Renderer() {};

public:
  virtual ~Renderer() = default;

  virtual void onUpdate(double ts) = 0;
  virtual bool onEvent(const Event &e) = 0;

  /* Creates an abstract Renderer backend
   *
   * @returns  Renderer Backend
   * @throws std::runtime_error if Window Surface cannot be created
   */
  static std::unique_ptr<Renderer> create(const Window &window);
};

} // namespace Froth
