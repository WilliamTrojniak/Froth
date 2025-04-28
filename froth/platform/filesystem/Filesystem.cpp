#include "Filesystem.h"

#include "core/logger/Logger.h"
#include <exception>
#include <fstream>

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

} // namespace Froth::Filesystem
