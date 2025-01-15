#pragma once

#include "platform/window/Window.h"
#include <memory>
namespace Froth {

class Renderer {
protected:
  Renderer() {};

public:
  virtual ~Renderer() = default;

  static std::unique_ptr<Renderer> create() noexcept;
  virtual bool init(const Window &window) noexcept = 0;
};

} // namespace Froth
