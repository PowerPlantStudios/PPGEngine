#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/device_object.h"
#include "rhi/rhi_types.h"

namespace PPGE
{
struct SamplerDesc : public DeviceObjectDesc
{
    // Filtering method to use when sampling a texture for minification
    FilterType min_filter = FilterType::FILTER_LINEAR;
    // Filtering method to use when sampling a texture for magnification
    FilterType mag_filter = FilterType::FILTER_LINEAR;
    // Filtering method to use when sampling a texture for mip levels
    FilterType mip_filter = FilterType::FILTER_LINEAR;
    // Method to use for resolving a u texture coordinate that is outside the 0 to 1 range
    TextureAddressType address_u = TextureAddressType::TEXTURE_ADDRESS_CLAMP;
    // Method to use for resolving a v texture coordinate that is outside the 0 to 1 range
    TextureAddressType address_v = TextureAddressType::TEXTURE_ADDRESS_CLAMP;
    // Method to use for resolving a w texture coordinate that is outside the 0 to 1 range
    TextureAddressType address_w = TextureAddressType::TEXTURE_ADDRESS_CLAMP;
    // Offset from the calculated mipmap level. For example, if sampler calculates that a
    // texture should be sampled at mipmap level 3.1 and MipLODBias is 2.3, then the texture
    // will be sampled at mipmap level 5.4.
    float mip_lod_bias = 0.0f;
    // Clamping value used if FILTER_ANISOTROPIC or FILTER_COMPERSION_ANISOTROPIC is 
    // specified in Filter. Valid values are between 1 and 16.
    uint32_t max_anisotropy = 1;
    // A function that compares sampled data against existing sampled data.
    // The function options are listed in ComperisonFuncType
    ComperisonFuncType comperison_func = ComperisonFuncType::COMPARISON_FUNC_NEVER;
    // Border color to use if TEXTURE_ADDRESS_BORDER is specified for address_u, address_v,
    // or address_w. Range must be between 0.0 and 1.0 inclusive.
    float border_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    // Lower end of the mipmap range to clamp access to, where 0 is the largest and most 
    // detailed mipmap level and any level higher than that is less detailed.
    float min_lod = -3.402823466e+38f;
    // Upper end of the mipmap range to clamp access to, where 0 is the largest and most 
    // detailed mipmap level and any level higher than that is less detailed. This value 
    // must be greater than or equal to MinLOD. To have no upper limit on LOD set this to 
    // a large value
    float max_lod = 3.402823466e+38F;
};

class PPGE_API PPGESampler : public PPGEDeviceObject
{
  public:
    virtual const SamplerDesc &GetDesc() const = 0;
};
} // namespace PPGE
