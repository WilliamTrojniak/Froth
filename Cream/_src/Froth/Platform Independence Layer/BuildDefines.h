#pragma once

// Debugging specific defines
#if defined(FROTH_DEBUG)
#define FROTH_ENABLE_ASSERTIONS

#elif defined(FROTH_RELEASE)

#elif defined(FROTH_DISTRIBUTION)

#endif

// Platform specific defines

#if defined(FROTH_PLATFORM_WINDOWS)
#define FROTH_DEBUGBREAK() __debugbreak()
#endif