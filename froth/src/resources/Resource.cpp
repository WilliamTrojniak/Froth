#include "Resource.h"

namespace Froth {

static std::string_view ResourceTypeToString(ResourceType type) {
  switch (type) {
  case ResourceType::None:
    return "ResourceType::None";
  case ResourceType::Texture:
    return "ResourceType::Texture2D";
  default:
    return "ResourceType::<Invalid>";
  }
}
} // namespace Froth
