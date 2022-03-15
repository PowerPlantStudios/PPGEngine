#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/device_object.h"

namespace PPGE
{
class PPGETexture;

struct TextureViewDesc : public DeviceObjectDesc
{
    ResourceViewType texture_view_type = ResourceViewType::RESOURCE_VIEW_UNDEFINED;
    ResourceDimensionType resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_UNDEFINED;
    TextureFormatType format = TextureFormatType::TEXTURE_FORMAT_UNKNOWN;
    uint32_t most_detailed_mip = 0;
    uint32_t mip_levels_num = 0;
    union {
        uint32_t first_array_slice = 0;
        uint32_t first_depth_slice;
    };
    union {
        uint32_t array_slices_num = 0;
        uint32_t depth_slices_num;
    };
};

class PPGE_API PPGETextureView : public PPGEDeviceObject
{
  public:
    virtual const TextureViewDesc &GetDesc() const = 0;

    virtual std::shared_ptr<PPGETexture> GetTexture() const = 0;
};
} // namespace PPGE
