#include "Filesystem.h"

#include "src/core/logger/Logger.h"
#include <exception>
#include <fstream>

#include <stb/stb_image.h>

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

} // namespace Froth::Filesystem
