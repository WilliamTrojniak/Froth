#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "vulkan/vulkan_core.h"
#include <cstddef>
#include <cstdint>
#include <tuple>
#include <type_traits>
#include <vector>

namespace Froth {

template <typename T>
struct VulkanFormat;

template <>
struct VulkanFormat<float> {
  static constexpr VkFormat value = VK_FORMAT_R32_SFLOAT;
};

template <>
struct VulkanFormat<glm::vec2> {
  static constexpr VkFormat value = VK_FORMAT_R32G32_SFLOAT;
};

template <>
struct VulkanFormat<glm::vec3> {
  static constexpr VkFormat value = VK_FORMAT_R32G32B32_SFLOAT;
};

template <>
struct VulkanFormat<glm::vec4> {
  static constexpr VkFormat value = VK_FORMAT_R32G32B32A32_SFLOAT;
};

struct VertexInputDescription {
  std::vector<VkVertexInputBindingDescription> bindings;
  std::vector<VkVertexInputAttributeDescription> attributes;

  VkPipelineVertexInputStateCreateInfo getInfo() const {
    VkPipelineVertexInputStateCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    info.vertexBindingDescriptionCount = static_cast<uint32_t>(bindings.size());
    info.pVertexBindingDescriptions = bindings.data();
    info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size());
    info.pVertexAttributeDescriptions = attributes.data();
    return info;
  }
};

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
  glm::vec2 texCoord;

  static VertexInputDescription getInputDescription(uint32_t binding = 0) {
    VertexInputDescription desc;

    VkVertexInputBindingDescription bindingDesc{};
    bindingDesc.binding = 0;
    bindingDesc.stride = sizeof(Vertex);
    bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    desc.bindings.push_back(bindingDesc);

    desc.attributes.resize(3);
    desc.attributes[0].binding = 0;
    desc.attributes[0].location = 0;
    desc.attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    desc.attributes[0].offset = offsetof(Vertex, pos);

    desc.attributes[1].binding = 0;
    desc.attributes[1].location = 1;
    desc.attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    desc.attributes[1].offset = offsetof(Vertex, color);

    desc.attributes[2].binding = 0;
    desc.attributes[2].location = 2;
    desc.attributes[2].format = VK_FORMAT_R32G32_SFLOAT;
    desc.attributes[2].offset = offsetof(Vertex, texCoord);

    return desc;
  }
};

} // namespace Froth
