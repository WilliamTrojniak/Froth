#include "VulkanImage.h"
#include "src/resources/Texture.h"

namespace Froth {

class VulkanTexture : public VulkanImage, public Texture {
public:
  VulkanTexture() = default;
  VulkanTexture(const VkExtent3D &extent, const VkFormat format)
      : VulkanImage(CreateInfo{
            .extent = extent,
            .format = format,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            .memPropFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT}) {
  }
};

} // namespace Froth
