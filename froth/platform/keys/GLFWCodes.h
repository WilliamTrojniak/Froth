#pragma once

#include "platform/keys/Keycodes.h"
#include "platform/keys/Mousecodes.h"
#include <unordered_map>
namespace Froth {
extern const std::unordered_map<int, KeyCode> GLFWKeyCodes;
extern const std::unordered_map<int, MouseCode> GLFWMouseCodes;
} // namespace Froth
