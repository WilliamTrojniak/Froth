#pragma once

#include "platform/window/Window.h"
namespace Froth {

class Renderer {
public:
  virtual ~Renderer() = default;

  static Renderer &getInstance() noexcept;
  static bool init(std::shared_ptr<Window> &window) noexcept;
  static void shutdown() noexcept;

protected:
  Renderer() = default;
};

} // namespace Froth
