#include "ResourceManager.h"

namespace Froth {

std::shared_ptr<Resource> ResourceManager::getResource(ResourceHandle handle) {
  if (!isHandleValid(handle))
    return nullptr;

  std::shared_ptr<Resource> pResource;
  if (isHandleLoaded(handle)) {
    pResource = m_LoadedResources.at(handle);
  } else {
    const ResourceMetadata &metadata = getMetadata(handle);
    // asset =
  }

  return pResource;
}

ResourceMetadata ResourceManager::getMetadata(ResourceHandle handle) {
  static ResourceMetadata s_NullMetadata{};
  auto it = m_ResourceRegistry.find(handle);
  if (it == m_ResourceRegistry.end()) {
    return s_NullMetadata;
  }

  return it->second;
}

ResourceType ResourceManager::getResourceType(ResourceHandle handle) const {
  if (!isHandleValid(handle))
    return ResourceType::None;

  return m_ResourceRegistry.at(handle).Type;
}

bool ResourceManager::isHandleValid(ResourceHandle handle) const {
  return handle != 0 && m_ResourceRegistry.find(handle) != m_ResourceRegistry.end();
}

bool ResourceManager::isHandleLoaded(ResourceHandle handle) const {
  return m_LoadedResources.find(handle) != m_LoadedResources.end();
}

} // namespace Froth
