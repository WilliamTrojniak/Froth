#pragma once

#include "Resource.h"

namespace Froth {

class Texture : public Resource {
  virtual ResourceType type() const noexcept override final { return ResourceType::Texture; }
};

} // namespace Froth
