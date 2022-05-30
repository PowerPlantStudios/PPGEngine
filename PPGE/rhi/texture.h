#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/device_object.h"
#include "rhi/rhi_types.h"
#include "rhi/texture_view.h"

namespace PPGE
{
struct TextureDesc : public DeviceObjectDesc
{
    ResourceDimensionType resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_UNDEFINED;
    uint32_t width = 0;
    uint32_t height = 0;
    union {
        // 1D and 2D textures can be composed of slices (array of textures)
        uint32_t array_size = 1;
        // 3D textures with width x height x depth dimension
        uint32_t depth;
    };
    uint32_t mip_levels = 1;
    // 2D texture or 2D texture arrays can be multi sampled
    uint32_t sample_count = 1;
    TextureFormatType format_type = TextureFormatType::TEXTURE_FORMAT_UNKNOWN;
    UsageType usage = UsageType::USAGE_DEFAULT;
    BindFlags bind_flags = BindFlags::BIND_NONE;
    CPUAccessFlags cpu_access_flags = CPUAccessFlags::CPU_ACCESS_NONE;
    uint32_t misc_flags = 0;
};

enum class TextureFileFormat : uint8_t
{
    UNKNOWN = 0,
    DDS,
    JPEG,
    PNG,
    TIFF
};

struct TextureData
{
    const void *data_ptr = nullptr;
    uint64_t data_size = 0;
    uint32_t pitch = 0;
    uint32_t slice_pitch = 0;
};

struct TextureCreateDesc
{
    TextureDesc desc;
    
    const char *resource_path = nullptr;
    TextureFileFormat file_format = TextureFileFormat::UNKNOWN;

    TextureData *subresource = nullptr;
    uint32_t subresource_num = 0;
    bool is_compressed = false;
};

class PPGE_API PPGETexture : public PPGEDeviceObject
{
  public:
    virtual const TextureDesc &GetDesc() const = 0;

    virtual std::shared_ptr<PPGETextureView> CreateView(const TextureViewDesc &desc) = 0;

    virtual std::shared_ptr<PPGETextureView> GetDefaultView() = 0;

    virtual size_t GetGenericHandle() = 0;
};
} // namespace PPGE
