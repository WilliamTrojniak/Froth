#pragma once
#define STB_IMAGE_IMPLEMENTATION


// General Defines
#define FROTH_ENGINE_NAME "FROTH"
#define FROTH_VERSION_MAJOR 0
#define FROTH_VERSION_MINOR 0
#define FROTH_VERSION_REVISION 1


// Debugging specific defines
#if defined(FROTH_DEBUG)
#define FROTH_ENABLE_ASSERTIONS
#define FROTH_ENABLE_VKVALIDATIONLAYER

#elif defined(FROTH_RELEASE)

#elif defined(FROTH_DISTRIBUTION)

#endif

// Platform specific defines

#if defined(FROTH_PLATFORM_WINDOWS)
#define FROTH_DEBUGBREAK() __debugbreak()
#define FROTH_WINDOWAPI_WIN
#define VK_USE_PLATFORM_WIN32_KHR
#define FROTH_GRAPHICSAPI_VK
#endif