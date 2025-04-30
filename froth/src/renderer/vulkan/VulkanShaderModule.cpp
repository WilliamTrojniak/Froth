#include "VulkanShaderModule.h"
#include "VulkanDevice.h"
#include "VulkanRenderer.h"
#include "src/core/logger/Logger.h"
#include <vector>

namespace Froth {

VulkanShaderModule::VulkanShaderModule(const std::vector<char> &code) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  if (vkCreateShaderModule(VulkanRenderer::context().device, &createInfo, VulkanRenderer::context().instance.allocator(), &m_ShaderModule) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create shader module");
  }
}

VulkanShaderModule::~VulkanShaderModule() {
  cleanup();
}

void VulkanShaderModule::cleanup() {
  if (m_ShaderModule) {
    vkDestroyShaderModule(VulkanRenderer::context().device, m_ShaderModule, VulkanRenderer::context().instance.allocator());
    m_ShaderModule = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Shader Module")
  }
}

} // namespace Froth
