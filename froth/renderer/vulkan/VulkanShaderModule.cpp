#include "VulkanShaderModule.h"
#include "core/logger/Logger.h"
#include "renderer/vulkan/VulkanDevice.h"
#include <vector>

namespace Froth {

VulkanShaderModule::VulkanShaderModule(const VulkanDevice &device, const std::vector<char> &code)
    : m_Device(device) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  if (vkCreateShaderModule(device, &createInfo, device.instance().allocator(), &m_ShaderModule) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create shader module");
  }
}

VulkanShaderModule::~VulkanShaderModule() {
  cleanup();
}

void VulkanShaderModule::cleanup() {
  if (m_ShaderModule) {
    vkDestroyShaderModule(m_Device, m_ShaderModule, m_Device.instance().allocator());
    m_ShaderModule = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Shader Module")
  }
}

} // namespace Froth
