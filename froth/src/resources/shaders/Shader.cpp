#include "Shader.h"
#include "src/renderer/vulkan/VulkanShaderModule.h"
#include <vector>

namespace Froth {

Shader::Shader(const std::vector<char> &code, Stage stage)
    : m_Code(code), m_Stage(stage) {
}

const VulkanShaderModule &Shader::getVulkanShaderModule() {
  if (!m_VulkanShaderModule.has_value()) {
    VkShaderStageFlagBits stage;
    switch (m_Stage) {
    case Stage::VERTEX:
      stage = VK_SHADER_STAGE_VERTEX_BIT;
      break;
    case Stage::FRAGMENT:
      stage = VK_SHADER_STAGE_FRAGMENT_BIT;
      break;
    }

    m_VulkanShaderModule.emplace(m_Code, stage);
  }

  return m_VulkanShaderModule.value();
}

} // namespace Froth
