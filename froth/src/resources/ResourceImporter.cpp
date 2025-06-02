#include "ResourceImporter.h"
#include "TextureImporter.h"
#include "src/core/logger/Logger.h"

namespace Froth {

std::shared_ptr<Resource> ResourceImporter::ImportResource(ResourceHandle handle, const ResourceMetadata &metadata) {
  switch (metadata.Type) {
  case ResourceType::Texture:
    return TextureImporter::ImportTexture2D(handle, metadata);
  case ResourceType::None:
  default:
    FROTH_ERROR("No importer available for resource type: %s", Resource::ResourceTypeToString(metadata.Type));
    return nullptr;
  }
}

} // namespace Froth
