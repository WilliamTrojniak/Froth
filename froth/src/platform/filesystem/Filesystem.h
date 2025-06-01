#pragma once

#include "src/renderer/vulkan/VulkanVertex.h"
#include <filesystem>
#include <vector>

#include <tiny_obj_loader.h>

namespace Froth::Filesystem {

std::vector<char> readFile(const std::filesystem::path &filename);

void *loadImage(const char *path, int &width, int &height);
void freeImage(void *data);

bool loadObj(const char *path, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices);

} // namespace Froth::Filesystem
