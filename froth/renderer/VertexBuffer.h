#pragma once

#include <cstddef>

namespace Froth {

class VertexBuffer {
protected:
  VertexBuffer() {};

public:
  virtual ~VertexBuffer() = default;

  virtual void bind() = 0;
  virtual void write(size_t sizeBytes, const void *vertexData) = 0;
};

} // namespace Froth
