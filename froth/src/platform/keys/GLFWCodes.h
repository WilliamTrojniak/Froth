#pragma once

#include "src/platform/keys/Keycodes.h"
#include "src/platform/keys/Mousecodes.h"
#include <unordered_map>
namespace Froth {
extern const std::unordered_map<int, KeyCode> GLFWKeyCodes;
extern const std::unordered_map<int, MouseCode> GLFWMouseCodes;
} // namespace Froth
