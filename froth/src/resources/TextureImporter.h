#pragma once

#include "Texture.h"
#include "src/resources/ResourceMetadata.h"

#include <filesystem>

namespace Froth {

class TextureImporter {
public:
  static std::shared_ptr<Texture> ImportTexture2D(ResourceHandle handle, const ResourceMetadata &metada);
  static std::shared_ptr<Texture> LoadTexture2D(const std::filesystem::path &path);
};

} // namespace Froth
