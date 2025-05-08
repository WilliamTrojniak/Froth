#include "VulkanShaderModule.h"
#include "VulkanDevice.h"
#include "src/core/logger/Logger.h"
#include "src/renderer/vulkan/VulkanContext.h"
#include <vector>

namespace Froth {

VulkanShaderModule::VulkanShaderModule(const std::vector<char> &code, VkShaderStageFlagBits stage)
    : m_Stage(stage) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  VulkanContext &vctx = VulkanContext::get();
  if (vkCreateShaderModule(vctx.device(), &createInfo, vctx.allocator(), &m_ShaderModule) != VK_SUCCESS) {
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
    VulkanContext &vctx = VulkanContext::get();
    vkDestroyShaderModule(vctx.device(), m_ShaderModule, vctx.allocator());
    m_ShaderModule = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Shader Module")
  }
}

} // namespace Froth
