#include "Filesystem.h"

#include "src/core/logger/Logger.h"
#include <algorithm>
#include <exception>
#include <fstream>

#include <stb/stb_image.h>
#include <unordered_map>

namespace Froth::Filesystem {

std::vector<char> readFile(const std::filesystem::path &filename) {
  std::vector<char> buffer;
  try {
    size_t filesize = std::filesystem::file_size(filename);
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
      FROTH_ERROR("Could not open file %s", filename.c_str());
    }
    buffer.resize(filesize);
    file.seekg(0);
    file.read(buffer.data(), filesize);
    file.close();

  } catch (std::exception &e) {
    FROTH_ERROR("Exception while reading file: %s", e.what())
  }

  return buffer;
}

void *loadImage(const char *path, int &width, int &height) {
  int texChannels;
  void *pixels = stbi_load(path, &width, &height, &texChannels, STBI_rgb_alpha);
  if (!pixels) {
    width = 0;
    height = 0;
    return nullptr;
  }

  return pixels;
}

void freeImage(void *data) {
  stbi_image_free(data);
}

bool loadObj(const char *path, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path)) {
    FROTH_WARN("Failed to open object file");
    return false;
  }

  for (const tinyobj::shape_t &shape : shapes) {
    for (const auto &index : shape.mesh.indices) {
      Vertex vertex{};
      vertex.pos = {
          attrib.vertices[3 * index.vertex_index + 0],
          attrib.vertices[3 * index.vertex_index + 1],
          attrib.vertices[3 * index.vertex_index + 2],
      };
      vertex.uv = {
          attrib.texcoords[2 * index.texcoord_index + 0],
          1.0f - attrib.texcoords[2 * index.texcoord_index + 1],
      };
      vertex.color = {1.0f, 1.0f, 1.0f};

      // if (std::find(vertices.begin(), vertices.end(), vertex) == vertices.end()) {
      // indices.push_back(vertices.size());
      // }

      vertices.push_back(vertex);
      indices.push_back(indices.size());
    }
  }
  return true;
}

} // namespace Froth::Filesystem
