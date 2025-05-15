#pragma once

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

namespace Froth {

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
  glm::vec2 uv;

  bool operator==(const Vertex &other) {
    return pos == other.pos &&
           color == other.color &&
           uv == other.uv;
  }
};

} // namespace Froth
