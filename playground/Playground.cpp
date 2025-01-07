#include "vulkan/vulkan_core.h"
#define GLFW_INCLUDE_VULKAN

#include "core/events/ApplicationEvent.h"
#include "core/events/Event.h"
#include "core/events/EventDispatcher.h"
#include "glm/glm.hpp"
#include "platform/window/Window.h"
#include <GLFW/glfw3.h>
#include <array>
#include <fstream>
#include <functional>
#include <limits>
#include <optional>
#include <set>
#include <string>
#include <sys/types.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "core/Entrypoint.h"

class VulkanTriangle : public Froth::Layer {
public:
  VulkanTriangle(const Froth::Window &window) : m_Window(window) {}
  virtual void onAttach() override {
    m_Instance = createVkInstance();
    m_Surface = createSurface(m_Instance, m_Window);
    m_PhysicalDevice = pickPhysicalDevice(m_Instance, m_Surface, m_DeviceExtensions);
    m_Device = createLogicalDevice(m_PhysicalDevice, m_Surface, m_DeviceExtensions);
    m_SwapchainInfo = createSwapchain(m_Device, m_PhysicalDevice, m_Surface, m_Window);
    m_RenderPass = createRenderPass(m_Device.device, m_SwapchainInfo);
    m_Framebuffers = createFramebuffers(m_Device.device, m_SwapchainInfo, m_RenderPass);
    m_Pipeline = createPipeline(m_Device.device, m_SwapchainInfo, m_RenderPass);
    m_CommandPool = createCommandPool(m_Device);
    m_CommandBuffers = createCommandBuffers(m_Device.device, m_CommandPool, MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      m_ImageAvailableSemaphores.push_back(createSemaphore(m_Device.device));
      m_RenderFinishedSemaphores.push_back(createSemaphore(m_Device.device));
      m_FrameInFlightFences.push_back(createFence(m_Device.device, true));
    }

    VkDeviceSize vertexBufferSize = sizeof(Vertex) * vertices.size();
    m_VertexBuffer = createVertexBuffer(m_Device.device, vertexBufferSize);
    m_VertexBufferMemory = allocateBufferMemory(m_Device.device, m_PhysicalDevice, m_VertexBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if (vkBindBufferMemory(m_Device.device, m_VertexBuffer, m_VertexBufferMemory, 0) != VK_SUCCESS) {
      throw std::runtime_error("failed to bind vertex buffer memory");
    }

    VkBuffer stagingBuffer = createBuffer(m_Device.device, vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    VkDeviceMemory stagingBufferMemory = allocateBufferMemory(m_Device.device, m_PhysicalDevice, stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (vkBindBufferMemory(m_Device.device, stagingBuffer, stagingBufferMemory, 0) != VK_SUCCESS) {
      throw std::runtime_error("failed to bind staging buffer memory");
    }

    void *data;
    vkMapMemory(m_Device.device, stagingBufferMemory, 0, vertexBufferSize, 0, &data);
    memcpy(data, vertices.data(), vertexBufferSize);
    vkUnmapMemory(m_Device.device, stagingBufferMemory);

    copyBuffer(m_Device, stagingBuffer, m_VertexBuffer, vertexBufferSize, m_CommandPool);
    vkDestroyBuffer(m_Device.device, stagingBuffer, nullptr);
    vkFreeMemory(m_Device.device, stagingBufferMemory, nullptr);

    VkDeviceSize indexBufferSize = sizeof(indices[0]) * indices.size();
    m_IndexBuffer = createBuffer(m_Device.device, indexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    m_IndexBufferMemory = allocateBufferMemory(m_Device.device, m_PhysicalDevice, m_IndexBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if (vkBindBufferMemory(m_Device.device, m_IndexBuffer, m_IndexBufferMemory, 0) != VK_SUCCESS) {
      throw std::runtime_error("failed to bind index buffer memory");
    }

    VkBuffer indexStagingBuffer = createBuffer(m_Device.device, indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    VkDeviceMemory indexStagingBufferMemory = allocateBufferMemory(m_Device.device, m_PhysicalDevice, indexStagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (vkBindBufferMemory(m_Device.device, indexStagingBuffer, indexStagingBufferMemory, 0) != VK_SUCCESS) {
      throw std::runtime_error("failed to bind index staging buffer memory");
    }
    vkMapMemory(m_Device.device, indexStagingBufferMemory, 0, indexBufferSize, 0, &data);
    memcpy(data, indices.data(), indexBufferSize);
    vkUnmapMemory(m_Device.device, indexStagingBufferMemory);
    copyBuffer(m_Device, indexStagingBuffer, m_IndexBuffer, indexBufferSize, m_CommandPool);
    vkDestroyBuffer(m_Device.device, indexStagingBuffer, nullptr);
    vkFreeMemory(m_Device.device, indexStagingBufferMemory, nullptr);
  }

  virtual void onDetatch() override {
    vkDeviceWaitIdle(m_Device.device);

    vkDestroyBuffer(m_Device.device, m_IndexBuffer, nullptr);
    vkFreeMemory(m_Device.device, m_IndexBufferMemory, nullptr);

    vkDestroyBuffer(m_Device.device, m_VertexBuffer, nullptr);
    vkFreeMemory(m_Device.device, m_VertexBufferMemory, nullptr);

    cleanupSwapchain();

    vkDestroyPipeline(m_Device.device, m_Pipeline.pipeline, nullptr);
    vkDestroyPipelineLayout(m_Device.device, m_Pipeline.layout, nullptr);
    vkDestroyRenderPass(m_Device.device, m_RenderPass, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      vkDestroySemaphore(m_Device.device, m_ImageAvailableSemaphores[i], nullptr);
      vkDestroySemaphore(m_Device.device, m_RenderFinishedSemaphores[i], nullptr);
      vkDestroyFence(m_Device.device, m_FrameInFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(m_Device.device, m_CommandPool, nullptr);
    vkDestroyDevice(m_Device.device, nullptr);
    vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
    vkDestroyInstance(m_Instance, nullptr);
  }
  virtual void onUpdate(double ts) override {
    vkWaitForFences(m_Device.device, 1, &m_FrameInFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(m_Device.device, m_SwapchainInfo.swapchain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      m_FramebufferResized = false;
      recreateSwapchain();
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("Failed to acquire swapchain image");
    }
    vkResetFences(m_Device.device, 1, &m_FrameInFlightFences[m_CurrentFrame]);

    vkResetCommandBuffer(m_CommandBuffers[m_CurrentFrame], 0);
    recordCommandBuffer(m_CommandBuffers[m_CurrentFrame], m_RenderPass, m_Pipeline.pipeline, m_SwapchainInfo, m_Framebuffers[imageIndex],
                        m_VertexBuffer, m_IndexBuffer, indices.size());

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &m_ImageAvailableSemaphores[m_CurrentFrame];
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentFrame];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &m_RenderFinishedSemaphores[m_CurrentFrame];

    if (vkQueueSubmit(m_Device.graphicsQueue.queue, 1, &submitInfo, m_FrameInFlightFences[m_CurrentFrame]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to submit draw command buffer");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &m_RenderFinishedSemaphores[m_CurrentFrame];

    VkSwapchainKHR swapchains[] = {m_SwapchainInfo.swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapchains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    result = vkQueuePresentKHR(m_Device.presentQueue.queue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_FramebufferResized) {
      recreateSwapchain();
    } else if (result != VK_SUCCESS) {
      throw std::runtime_error("Failed to present swap chain image");
    }

    m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
  }

  virtual bool onEvent(const Froth::Event &e) override {
    Froth::EventDispatcher d(e);
    d.dispatch<Froth::WindowResizeEvent>(std::bind(&VulkanTriangle::onWindowResize, this, std::placeholders::_1));

    return d.isHandled();
  }

  bool onWindowResize(const Froth::WindowResizeEvent &e) {
    m_FramebufferResized = true;
    return false;
  }

private:
  struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getVkVertexInputBindingDescription() {
      VkVertexInputBindingDescription bindingDesc{};
      bindingDesc.binding = 0;
      bindingDesc.stride = sizeof(Vertex);
      bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

      return bindingDesc;
    }
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
      std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
      attributeDescriptions[0].binding = 0;
      attributeDescriptions[0].location = 0;
      attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
      attributeDescriptions[0].offset = offsetof(Vertex, pos);

      attributeDescriptions[1].binding = 0;
      attributeDescriptions[1].location = 1;
      attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
      attributeDescriptions[1].offset = offsetof(Vertex, color);

      return attributeDescriptions;
    }
  };

  const std::vector<Vertex> vertices = {
      {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
      {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
  };
  const std::vector<uint16_t> indices = {
      0,
      1,
      2,
      2,
      3,
      0,
  };

  const uint32_t MAX_FRAMES_IN_FLIGHT = 2;
  uint32_t m_CurrentFrame = 0;
  bool m_FramebufferResized = false;
  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  struct SwapchainInfo {
    VkSwapchainKHR swapchain;
    VkFormat format;
    VkExtent2D extent;
    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
  };

  const std::vector<const char *>
      m_DeviceExtensions{
          VK_KHR_SWAPCHAIN_EXTENSION_NAME,
          // TODO: Only enable on MacOS
          "VK_KHR_portability_subset",
      };
  const Froth::Window &m_Window;
  VkInstance m_Instance;
  VkSurfaceKHR m_Surface;
  VkPhysicalDevice m_PhysicalDevice;
  struct QueueConfig {
    VkQueue queue;
    uint32_t index;
  };
  struct DeviceConfig {
    VkDevice device;
    QueueConfig graphicsQueue;
    QueueConfig presentQueue;
  } m_Device;

  SwapchainInfo m_SwapchainInfo;
  VkRenderPass m_RenderPass;
  struct PipelineInfo {
    VkPipeline pipeline;
    VkPipelineLayout layout;
  } m_Pipeline;
  std::vector<VkFramebuffer> m_Framebuffers;

  VkCommandPool m_CommandPool;
  std::vector<VkCommandBuffer> m_CommandBuffers;
  std::vector<VkSemaphore> m_ImageAvailableSemaphores;
  std::vector<VkSemaphore> m_RenderFinishedSemaphores;
  std::vector<VkFence> m_FrameInFlightFences;

  VkBuffer m_VertexBuffer;
  VkDeviceMemory m_VertexBufferMemory;

  VkBuffer m_IndexBuffer;
  VkDeviceMemory m_IndexBufferMemory;

private:
  static bool hasExtensions(const std::vector<const char *> &extensions) {
    uint32_t availableExtensionsCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionsCount,
                                           nullptr);
    std::vector<VkExtensionProperties> availableExtensions(
        availableExtensionsCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionsCount,
                                           availableExtensions.data());

    for (const char *reqEx : extensions) {
      if (std::find_if(availableExtensions.begin(), availableExtensions.end(), [reqEx](const VkExtensionProperties &ex) {
            return strcmp(ex.extensionName, reqEx) == 0;
          }) == availableExtensions.end()) {
        std::cout << "Missing: " << reqEx << std::endl;
        return false;
      }
    }
    return true;
  }

  static bool deviceSupportsExtensions(VkPhysicalDevice device, const std::vector<const char *> extensions) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(extensions.begin(), extensions.end());
    for (const VkExtensionProperties &e : availableExtensions) {
      requiredExtensions.erase(e.extensionName);
    }

    return requiredExtensions.empty();
  }

  static bool hasValidationLayerSupport(std::vector<const char *> &layers) {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layer : layers) {
      if (std::find_if(availableLayers.begin(), availableLayers.end(), [layer](const VkLayerProperties &l) {
            return strcmp(layer, l.layerName) == 0;
          }) == availableLayers.end()) {
        return false;
      }
    }
    return true;
  }

  static VkInstance createVkInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "VulkanTriangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions =
        glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> requiredExtensions;
    for (uint32_t i = 0; i < glfwExtensionCount; i++) {
      requiredExtensions.emplace_back(glfwExtensions[i]);
    }

    // Required for MacOS
    // TODO: Only enable on MacOS
    requiredExtensions.emplace_back(
        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    if (!hasExtensions(requiredExtensions)) {
      throw std::runtime_error("Missing required extension(s) support");
    }

    createInfo.enabledExtensionCount =
        static_cast<uint32_t>(requiredExtensions.size());
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

    // TODO: Only enable validation layers in debug builds
    std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation",
    };

    if (!hasValidationLayerSupport(validationLayers)) {
      throw std::runtime_error("Unsupported validation layers");
    }

    createInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    VkInstance instance;
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create vulkan instance");
    }
    std::cerr << "Created Vulkan Instance" << std::endl;
    return instance;
  }
  static bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const std::vector<const char *> &extensions) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    QueueFamilyIndices indices = findQueueFamilies(device, surface);
    bool supportsExtensions = deviceSupportsExtensions(device, extensions);

    bool swapChainAdequate = false;
    if (supportsExtensions) {
      SwapChainSupportDetails swapChainDetails = querySwapChainSupport(device, surface);
      swapChainAdequate = !swapChainDetails.formats.empty() && !swapChainDetails.presentModes.empty();
    }

    // TODO: Check device properties and features;
    return indices.isComplete() && swapChainAdequate;
  }

  static VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, const std::vector<const char *> &extensions) {

    uint32_t deviceCount;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
      throw std::runtime_error("No available devices with Vulkan support");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    for (const VkPhysicalDevice &device : devices) {
      if (isDeviceSuitable(device, surface, extensions)) {
        physicalDevice = device;
        break;
      }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
      throw std::runtime_error("Could not find suitable GPU");
    }

    return physicalDevice;
  }

  struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
      return graphicsFamily.has_value() && presentFamily.has_value();
    }
  };

  static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    QueueFamilyIndices indices;

    int32_t i = 0;
    for (int32_t i = 0; i < queueFamilyCount; i++) {
      if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        indices.graphicsFamily = i;
      }

      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
      if (presentSupport)
        indices.presentFamily = i;

      if (indices.isComplete())
        break;
    }

    return indices;
  }

  static DeviceConfig createLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const std::vector<const char *> &extensions) {
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice, surface);
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value()};

    float queuePriority = 1.0;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
      VkDeviceQueueCreateInfo queueCreateInfo{};
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = queueFamily;
      queueCreateInfo.queueCount = 1;
      queueCreateInfo.pQueuePriorities = &queuePriority;
      queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    // TODO: Only enable validation layers in debug builds
    std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation",
    };

    if (!hasValidationLayerSupport(validationLayers)) {
      throw std::runtime_error("Unsupported validation layers");
    }

    deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();

    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = extensions.data();

    VkDevice device;
    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create logical device");
    }

    VkQueue graphicsQueue;
    VkQueue presentQueue;
    vkGetDeviceQueue(device, queueFamilyIndices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, queueFamilyIndices.presentFamily.value(), 0, &presentQueue);

    DeviceConfig cfg{};
    cfg.device = device;
    cfg.graphicsQueue = QueueConfig{graphicsQueue, queueFamilyIndices.graphicsFamily.value()};
    cfg.presentQueue = QueueConfig{presentQueue, queueFamilyIndices.presentFamily.value()};

    return cfg;
  }

  static VkSurfaceKHR createSurface(VkInstance instance, const Froth::Window &window) {
    VkSurfaceKHR surface;
    // TODO: Fix cast based on Window API
    if (glfwCreateWindowSurface(instance, static_cast<GLFWwindow *>(window.nativeWindow()), nullptr, &surface) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create surface");
    }
    return surface;
  }

  static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapChainSupportDetails details;

    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities) != VK_SUCCESS) {
      throw std::runtime_error("Failed to get physical device surface capabilities");
    }

    uint32_t formatCount;
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr) != VK_SUCCESS) {
      throw std::runtime_error("Failed to get surface formats");
    }
    details.formats.resize(formatCount);
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data()) != VK_SUCCESS) {
      throw std::runtime_error("Failed to get surface formats");
    }

    uint32_t presentModeCount;
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr) != VK_SUCCESS) {
      throw std::runtime_error("Failed to get surface present modes");
    }
    details.presentModes.resize(presentModeCount);
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data()) != VK_SUCCESS) {
      throw std::runtime_error("Failed to get surface present modes");
    }

    return details;
  }

  static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
    for (const VkSurfaceFormatKHR &format : availableFormats) {
      if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
        return format;
      }
    }
    return availableFormats[0];
  }

  static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
    for (const VkPresentModeKHR &mode : availablePresentModes) {
      if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
        return mode;
    }

    return VK_PRESENT_MODE_FIFO_KHR; // Guaranteed to be available
  }

  static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, const Froth::Window &window) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
      return capabilities.currentExtent;
    }

    VkExtent2D actualExtent;
    window.getFramebufferSize(actualExtent.width, actualExtent.height);

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    return actualExtent;
  }

  static SwapchainInfo createSwapchain(const DeviceConfig &deviceCfg, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const Froth::Window &window) {
    SwapChainSupportDetails swapChainSupportDetails = querySwapChainSupport(physicalDevice, surface);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupportDetails.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupportDetails.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupportDetails.capabilities, window);

    uint32_t imageCount = swapChainSupportDetails.capabilities.minImageCount + 1; // To avoid waiting on driver internal operations

    if (swapChainSupportDetails.capabilities.maxImageCount > 0 && imageCount > swapChainSupportDetails.capabilities.maxImageCount) {
      // Max image count of 0 is special value denoting no limit
      imageCount = swapChainSupportDetails.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
      createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = 2;
      createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
      createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      createInfo.queueFamilyIndexCount = 0;
      createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupportDetails.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    SwapchainInfo swapchain{};

    if (vkCreateSwapchainKHR(deviceCfg.device, &createInfo, nullptr, &swapchain.swapchain) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create swapchain");
    }

    if (vkGetSwapchainImagesKHR(deviceCfg.device, swapchain.swapchain, &imageCount, nullptr) != VK_SUCCESS) {
      throw std::runtime_error("Failed to retreive swapchain images");
    }
    swapchain.images.resize(imageCount);
    if (vkGetSwapchainImagesKHR(deviceCfg.device, swapchain.swapchain, &imageCount, swapchain.images.data()) != VK_SUCCESS) {
      throw std::runtime_error("Failed to retreive swapchain images");
    }

    swapchain.extent = extent;
    swapchain.format = surfaceFormat.format;
    swapchain.imageViews.resize(swapchain.images.size());

    for (uint32_t i = 0; i < swapchain.images.size(); i++) {
      VkImageViewCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      createInfo.image = swapchain.images[i];
      createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      createInfo.format = surfaceFormat.format;
      createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      createInfo.subresourceRange.baseMipLevel = 0;
      createInfo.subresourceRange.levelCount = 1;
      createInfo.subresourceRange.baseArrayLayer = 0;
      createInfo.subresourceRange.layerCount = 1;
      if (vkCreateImageView(deviceCfg.device, &createInfo, nullptr, &swapchain.imageViews[i]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create image view");
      }
    }

    return swapchain;
  }

  static std::vector<char> readFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open file");
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
  }

  static VkShaderModule createShaderModule(VkDevice device, const std::vector<char> &code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    VkShaderModule shader;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shader) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create shader module");
    }

    return shader;
  }

  static VkRenderPass createRenderPass(VkDevice device, const SwapchainInfo &swapchain) {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapchain.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    VkRenderPass renderPass;

    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create render pass");
    }

    return renderPass;
  }

  static PipelineInfo createPipeline(VkDevice device, const SwapchainInfo &swapchain, VkRenderPass renderPass) {
    std::vector<char> vertShaderCode = readFile("../playground/shaders/vert.spv");
    std::vector<char> fragShaderCode = readFile("../playground/shaders/frag.spv");

    VkShaderModule vertShaderModule = createShaderModule(device, vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(device, fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    VkVertexInputBindingDescription bindingDesc = Vertex::getVkVertexInputBindingDescription();
    std::array<VkVertexInputAttributeDescription, 2> vertexAttributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDesc;
    vertexInputInfo.vertexAttributeDescriptionCount = vertexAttributeDescriptions.size();
    vertexInputInfo.pVertexAttributeDescriptions = vertexAttributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
    inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = swapchain.extent.width;
    viewport.height = swapchain.extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapchain.extent;

    VkPipelineViewportStateCreateInfo viewportStateInfo{};
    viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateInfo.viewportCount = 1;
    viewportStateInfo.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizerInfo{};
    rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerInfo.depthClampEnable = VK_FALSE;
    rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizerInfo.lineWidth = 1.0f;
    rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizerInfo.depthBiasEnable = VK_FALSE;
    rasterizerInfo.depthBiasConstantFactor = 0.0f;
    rasterizerInfo.depthBiasClamp = VK_FALSE;
    rasterizerInfo.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisampleInfo{};
    multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleInfo.sampleShadingEnable = VK_FALSE;
    multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleInfo.minSampleShading = 1.0f;
    multisampleInfo.pSampleMask = nullptr;
    multisampleInfo.alphaToCoverageEnable = VK_FALSE;
    multisampleInfo.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
    colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendInfo.logicOpEnable = VK_FALSE;
    colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
    colorBlendInfo.attachmentCount = 1;
    colorBlendInfo.pAttachments = &colorBlendAttachment;
    colorBlendInfo.blendConstants[0] = 0.0f;
    colorBlendInfo.blendConstants[1] = 0.0f;
    colorBlendInfo.blendConstants[2] = 0.0f;
    colorBlendInfo.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.setLayoutCount = 0;
    layoutInfo.pSetLayouts = nullptr;
    layoutInfo.pushConstantRangeCount = 0;
    layoutInfo.pPushConstantRanges = nullptr;

    PipelineInfo pipeline;

    if (vkCreatePipelineLayout(device, &layoutInfo, nullptr, &pipeline.layout) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create pipeline layout");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
    pipelineInfo.pViewportState = &viewportStateInfo;
    pipelineInfo.pRasterizationState = &rasterizerInfo;
    pipelineInfo.pMultisampleState = &multisampleInfo;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlendInfo;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipeline.layout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline.pipeline) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create pipeline");
    }

    vkDestroyShaderModule(device, fragShaderModule, nullptr);
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
    return pipeline;
  }

  static std::vector<VkFramebuffer> createFramebuffers(VkDevice device, const SwapchainInfo &swapchain, VkRenderPass renderPass) {
    std::vector<VkFramebuffer> buffers(swapchain.imageViews.size());
    for (size_t i = 0; i < buffers.size(); i++) {
      VkImageView attachments[] = {swapchain.imageViews[i]};
      VkFramebufferCreateInfo framebufferInfo{};
      framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass = renderPass;
      framebufferInfo.attachmentCount = 1;
      framebufferInfo.pAttachments = attachments;
      framebufferInfo.width = swapchain.extent.width;
      framebufferInfo.height = swapchain.extent.height;
      framebufferInfo.layers = 1;

      if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &buffers[i]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create framebuffer");
      }
    }

    return buffers;
  }

  static VkCommandPool createCommandPool(DeviceConfig deviceCfg) {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = deviceCfg.graphicsQueue.index;

    VkCommandPool pool;
    if (vkCreateCommandPool(deviceCfg.device, &poolInfo, nullptr, &pool) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create command pool");
    }
    return pool;
  }

  static std::vector<VkCommandBuffer> createCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t count) {
    std::vector<VkCommandBuffer> buffers(count);
    for (size_t i = 0; i < buffers.size(); i++) {
      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.commandPool = commandPool;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandBufferCount = 1;

      VkCommandBuffer buffer;
      if (vkAllocateCommandBuffers(device, &allocInfo, &buffers[i]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffer");
      }
    }
    return buffers;
  }

  static void recordCommandBuffer(VkCommandBuffer commandBuffer, VkRenderPass renderpass, VkPipeline pipeline,
                                  const SwapchainInfo &swapchain, VkFramebuffer framebuffer, VkBuffer vertexBuffer,
                                  VkBuffer indexBuffer, VkDeviceSize indexCount) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("Failed to begin recording to command buffer");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderpass;
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapchain.extent;

    VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, offsets);

    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = swapchain.extent.width;
    viewport.height = swapchain.extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapchain.extent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      throw std::runtime_error("Failed to record command buffer");
    }
  }

  static VkSemaphore createSemaphore(VkDevice device) {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkSemaphore semaphore;
    if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create semaphore");
    }
    return semaphore;
  }
  static VkFence createFence(VkDevice device, bool signaled) {
    VkFenceCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    if (signaled) {
      semaphoreInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }

    VkFence fence;
    if (vkCreateFence(device, &semaphoreInfo, nullptr, &fence) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create fence");
    }
    return fence;
  }

  void cleanupSwapchain() {

    for (const VkFramebuffer &buffer : m_Framebuffers) {
      vkDestroyFramebuffer(m_Device.device, buffer, nullptr);
    }

    for (const VkImageView &imageView : m_SwapchainInfo.imageViews) {
      vkDestroyImageView(m_Device.device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(m_Device.device, m_SwapchainInfo.swapchain, nullptr);

    // TODO: Recreate render pass and pipeline
  }

  void recreateSwapchain() {
    vkDeviceWaitIdle(m_Device.device);

    cleanupSwapchain();
    m_SwapchainInfo = createSwapchain(m_Device, m_PhysicalDevice, m_Surface, m_Window);
    m_Framebuffers = createFramebuffers(m_Device.device, m_SwapchainInfo, m_RenderPass);
  }
  static VkBuffer createBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkBuffer buffer;
    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create vertex buffer");
    }
    return buffer;
  }

  static VkBuffer createVertexBuffer(VkDevice device, VkDeviceSize size) {
    return createBuffer(device, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
  }

  static uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
      if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
        return i;
      }
    }

    throw std::runtime_error("failed to find suitable memopry type");
  }

  static VkDeviceMemory allocateBufferMemory(VkDevice device, VkPhysicalDevice physicalDevice, VkBuffer buffer, VkMemoryPropertyFlags properties) {
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

    VkDeviceMemory memory;
    if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate buffer memory");
    }
    return memory;
  }

  static void copyBuffer(DeviceConfig deviceCfg, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool commandPool) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    if (vkAllocateCommandBuffers(deviceCfg.device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
      throw std::runtime_error("Failed to allocate command buffer");
    };

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("failed to begin command buffer");
    }

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      throw std::runtime_error("failed to end command buffer");
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    if (vkQueueSubmit(deviceCfg.graphicsQueue.queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
      throw std::runtime_error("Failed to submit to queue");
    }

    if (vkQueueWaitIdle(deviceCfg.graphicsQueue.queue) != VK_SUCCESS) {
      throw std::runtime_error("Failed to wait for graphics queue idle");
    }
    vkFreeCommandBuffers(deviceCfg.device, commandPool, 1, &commandBuffer);
  }
};

class Playground : public Froth::Application {
public:
  Playground() { pushLayer(std::make_unique<VulkanTriangle>(window())); }
};

Froth::Application *Froth::CreateApplication() { return new Playground(); }
