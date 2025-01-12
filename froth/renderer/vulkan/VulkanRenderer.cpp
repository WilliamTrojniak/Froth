#include "VulkanRenderer.h"
#include "Defines.h"
#include "core/logger/Logger.h"
#include "renderer/vulkan/VulkanDevice.h"
#include <set>

namespace Froth {

bool hasExtensions(const std::vector<const char *> &extensions) noexcept;
bool getRequiredExtensions(std::vector<const char *> &extensions) noexcept;
bool hasLayers(const std::vector<const char *> &layers) noexcept;

const char *VK_LAYER_KHRONOS_validation = "VK_LAYER_KHRONOS_validation";

bool VulkanRenderer::s_Initialized = false;

VulkanRenderer &VulkanRenderer::getInstance() {
  if (!s_Initialized) {
    // TODO: Maybe throw an error?
    FROTH_WARN("Retreived Vulkan renderer before initialization");
  }
  return getInstanceInt();
}
VulkanRenderer &VulkanRenderer::getInstanceInt() noexcept {
  static VulkanRenderer s_Instance;
  return s_Instance;
}

VulkanRenderer::~VulkanRenderer() {
  if (s_Initialized) {
    FROTH_WARN("Renderer not shutdown.")
  }
}

bool VulkanRenderer::init(std::shared_ptr<Window> &window) noexcept {
  VulkanRenderer &s_Instance = getInstanceInt();
  s_Instance.m_Context.window = window;
  // TODO: Configurable allocator
  if (!initInstance(nullptr, s_Instance.m_Context)) {
    s_Instance.shutdown();
    return false;
  }

  if (!window->createVulkanSurface(s_Instance.m_Context.instance, s_Instance.m_Context.allocator, s_Instance.m_Context.surface)) {
    s_Instance.shutdown();
    return false;
  }

  if (!VulkanDevice::create(s_Instance.m_Context.instance, s_Instance.m_Context.surface, s_Instance.m_Context.device)) {
    s_Instance.shutdown();
    return false;
  }

  FROTH_INFO("Initialized Vulkan Renderer")
  s_Initialized = true;
  return true;
}

void VulkanRenderer::shutdown() noexcept {
  s_Initialized = false;
  if (m_Context.surface != nullptr) {
    vkDestroySurfaceKHR(m_Context.instance, m_Context.surface, m_Context.allocator);
  }

  if (m_Context.instance != nullptr) {
    vkDestroyInstance(m_Context.instance, m_Context.allocator);
    m_Context.instance = nullptr;
  }
  FROTH_DEBUG("Shut down Vulkan Renderer")
}

bool VulkanRenderer::initInstance(const VkAllocationCallbacks *allocator, VulkanRendererContext &context) noexcept {
  context.allocator = allocator;

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

  return vkCreateInstance(&createInfo, context.allocator, &context.instance) == VK_SUCCESS;
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
