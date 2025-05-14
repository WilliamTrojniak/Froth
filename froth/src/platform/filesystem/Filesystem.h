#pragma once

#include <filesystem>
#include <vector>

namespace Froth::Filesystem {

std::vector<char> readFile(const std::filesystem::path &filename);

void *loadImage(const char *path, int &width, int &height);
void freeImage(void *data);

} // namespace Froth::Filesystem
