#pragma once

#include "Resource.h"
#include "ResourceMetadata.h"

namespace Froth {

class ResourceImporter {
public:
  static std::shared_ptr<Resource> ImportResource(ResourceHandle handle, const ResourceMetadata &metadata);
};

} // namespace Froth
