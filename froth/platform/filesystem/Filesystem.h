#pragma once

#include <filesystem>
#include <vector>

namespace Froth::Filesystem {

std::vector<char> readFile(const std::filesystem::path &filename);

} // namespace Froth::Filesystem
