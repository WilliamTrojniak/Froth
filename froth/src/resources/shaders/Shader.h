#pragma once

#include "src/renderer/vulkan/VulkanShaderModule.h"
#include <optional>
#include <vector>

namespace Froth {
class Shader {
public:
  enum class Stage {
    VERTEX,
    FRAGMENT
  };

  Shader(const std::vector<char> &code, Stage stage);

  const VulkanShaderModule &getVulkanShaderModule();

private:
  const std::vector<char> m_Code;
  const Stage m_Stage;
  std::optional<VulkanShaderModule> m_VulkanShaderModule;
};

} // namespace Froth
