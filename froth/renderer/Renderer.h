#pragma once

#include <memory>
namespace Froth {

class Renderer {
public:
  virtual ~Renderer() = default;

  static std::unique_ptr<Renderer> createRenderer();

protected:
  Renderer() = default;
};

} // namespace Froth
