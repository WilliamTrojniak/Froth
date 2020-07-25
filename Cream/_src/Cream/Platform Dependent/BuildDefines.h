#pragma once

// Debugging specific defines
#if defined(CREAM_DEBUG)
#define CREAM_ENABLE_ASSERTIONS

#elif defined(CREAM_RELEASE)

#elif defined(CREAM_DISTRIBUTION)

#endif

// Platform specific defines

#if defined(CREAM_PLATFORM_WINDOWS)
#define CREAM_DEBUGBREAK() __debugbreak()
#endif