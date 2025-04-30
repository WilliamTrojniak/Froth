#pragma once

#include <cstddef>
#include <cstdint>

namespace Froth {

class IndexBuffer {
protected:
  IndexBuffer() {};

public:
  virtual ~IndexBuffer() = default;

  virtual void bind() = 0;
  virtual void write(size_t numIndices, const uint32_t *indexData) = 0;
};

} // namespace Froth
