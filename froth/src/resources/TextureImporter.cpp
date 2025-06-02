#include "TextureImporter.h"
#include "src/core/logger/Logger.h"
#include "src/platform/filesystem/Filesystem.h"
#include "src/renderer/vulkan/VulkanTexture.h"

namespace Froth {

static std::shared_ptr<Texture> LoadTexture2D(const std::filesystem::path &path) {

  int width, height, channels;
  void *data = Filesystem::loadImage(path.c_str(), width, height, channels);

  if (data == nullptr) {
    FROTH_ERROR("TextureImporter::LoadTexture2D - Could not load texture from filepath %s", path.c_str());
    return nullptr;
  }

  VkExtent3D extent{
      .width = static_cast<uint32_t>(width),
      .height = static_cast<uint32_t>(height),
      .depth = 1,
  };
  std::shared_ptr<VulkanTexture> pTexture = std::make_shared<VulkanTexture>(extent, VK_FORMAT_R8G8B8A8_SRGB);
  // TODO: Upload data to the texture and transition the image layout

  Filesystem::freeImage(data);
  return pTexture;
}
} // namespace Froth
