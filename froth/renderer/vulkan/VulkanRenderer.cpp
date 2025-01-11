#include "VulkanRenderer.h"
#include "Defines.h"
#include <platform/window/Window.h>
#include <set>
#include <vector>

namespace Froth {

bool hasExtensions(const std::vector<const char *> &extensions) noexcept;
bool getRequiredExtensions(std::vector<const char *> &extensions) noexcept;
bool hasLayers(const std::vector<const char *> &layers) noexcept;

const char *VK_LAYER_KHRONOS_validation = "VK_LAYER_KHRONOS_validation";

VulkanRenderer::VulkanRendererContext VulkanRenderer::s_Context{
    nullptr,
    nullptr,
};
uint32_t VulkanRenderer::s_Count = 0;
bool VulkanRenderer::s_Initialized = false;

VulkanRenderer::VulkanRenderer() {
  if (!s_Initialized && !init()) {
    throw std::runtime_error("Failed to initialize Vulkan Renderer");
  }

  s_Count++;
}

VulkanRenderer::~VulkanRenderer() {
  s_Count--;
  if (s_Count == 0 && !shutdown()) {
    // TODO: Log that shutdown failed
  }
}

bool VulkanRenderer::init() noexcept {
  if (!initInstance(s_Context.instance)) {
    return false;
  }
  s_Initialized = true;
  return true;
}

bool VulkanRenderer::shutdown() noexcept {

  if (s_Context.instance != nullptr) {
    vkDestroyInstance(s_Context.instance, s_Context.allocator);
    s_Context.instance = nullptr;
  }
  return true;
}

bool VulkanRenderer::initInstance(VkInstance &instance) noexcept {
  VkApplicationInfo appInfo{VK_STRUCTURE_TYPE_APPLICATION_INFO};
  appInfo.pApplicationName = "App Name";                 // TODO: Make configurable
  appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0); // TODO: Make configurable
  appInfo.pEngineName = "Froth Game Engine";             // TODO: Make constant
  appInfo.engineVersion = VK_MAKE_VERSION(FROTH_VERSION_MAJOR, FROTH_VERSION_MINOR, FROTH_VERSION_PATCH);
  appInfo.apiVersion = VK_API_VERSION_1_3; // TODO: Setup in build settings

  // Extensions
  std::vector<const char *> extensions;
  if (!getRequiredExtensions(extensions)) {
    return false;
  }

  if (!hasExtensions(extensions)) {
    return false;
  }

  // Validation Layers
  // TODO: Only in debug builds
  const std::vector<const char *> validationLayers = {
      VK_LAYER_KHRONOS_validation,
  };

  if (!hasLayers(validationLayers)) {
    return false;
  }

  VkInstanceCreateInfo createInfo{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();
  // TODO: Only on MacOS
  createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
  createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
  createInfo.ppEnabledLayerNames = validationLayers.data();

  return vkCreateInstance(&createInfo, s_Context.allocator, &instance) == VK_SUCCESS;
}

bool getRequiredExtensions(std::vector<const char *> &extensions) noexcept {
  uint32_t requiredWindowExtensionsCount;
  const char **requiredWindowExtensions = Window::requiredVulkanExtensions(requiredWindowExtensionsCount);
  if (requiredWindowExtensions == nullptr) {
    return false;
  }

  extensions.resize(requiredWindowExtensionsCount);
  for (uint32_t i = 0; i < requiredWindowExtensionsCount; i++) {
    extensions[i] = requiredWindowExtensions[i];
  }

  // TODO: Only enable for MacOS
  extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

  return true;
}

bool hasExtensions(const std::vector<const char *> &extensions) noexcept {
  uint32_t availableExtensionsCount;
  if (vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionsCount, nullptr) != VK_SUCCESS) {
    return false;
  }

  std::vector<VkExtensionProperties> availableExtensions(availableExtensionsCount);
  if (vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionsCount, availableExtensions.data()) != VK_SUCCESS) {
    return false;
  };

  std::set<std::string> extensionsSet(extensions.begin(), extensions.end());
  for (const auto &e : availableExtensions) {
    extensionsSet.erase(e.extensionName);
  }

  return extensionsSet.empty();
}

bool hasLayers(const std::vector<const char *> &layers) noexcept {
  uint32_t availableLayerCount;
  if (vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr) != VK_SUCCESS) {
    return false;
  }

  std::vector<VkLayerProperties> availableLayers(availableLayerCount);
  if (vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers.data()) != VK_SUCCESS) {
    return false;
  };

  std::set<std::string> layerSet(layers.begin(), layers.end());
  for (const auto &l : availableLayers) {
    layerSet.erase(l.layerName);
  }

  return layerSet.empty();
}

} // namespace Froth
