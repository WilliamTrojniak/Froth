#pragma once

#include "Resource.h"
#include "ResourceMetadata.h"

#include <memory>
#include <unordered_map>

namespace Froth {

using ResourceRegistry = std::unordered_map<ResourceHandle, ResourceMetadata>;
using ResourceMap = std::unordered_map<ResourceHandle, std::shared_ptr<Resource>>;

class ResourceManager {
public:
  std::shared_ptr<Resource> getResource(ResourceHandle handle);
  ResourceMetadata getMetadata(ResourceHandle handle);
  ResourceType getResourceType(ResourceHandle handle) const;

  bool isHandleValid(ResourceHandle handle) const;
  bool isHandleLoaded(ResourceHandle handle) const;

private:
  ResourceRegistry m_ResourceRegistry;
  ResourceMap m_LoadedResources;
};

} // namespace Froth
