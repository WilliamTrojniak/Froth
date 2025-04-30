#include "VulkanShaderModule.h"
#include "VulkanDevice.h"
#include "VulkanRenderer.h"
#include "src/core/logger/Logger.h"
#include "vulkan/vulkan_core.h"
#include <vector>

namespace Froth {

VulkanShaderModule::VulkanShaderModule(const std::vector<char> &code, VkShaderStageFlagBits stage)
    : m_Stage(stage) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  if (vkCreateShaderModule(VulkanRenderer::context().device, &createInfo, VulkanRenderer::context().instance.allocator(), &m_ShaderModule) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create shader module");
  }
}

VkPipelineShaderStageCreateInfo VulkanShaderModule::pipelineStageInfo() const {
  VkPipelineShaderStageCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  createInfo.stage = m_Stage;
  createInfo.module = m_ShaderModule;
  createInfo.pName = "main";

  return createInfo;
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
