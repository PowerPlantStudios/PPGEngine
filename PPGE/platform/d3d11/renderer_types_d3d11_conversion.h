#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/ppge_enum.h"
#include "renderer/renderer_types.h"
#include "system/logger_system.h"

namespace PPGE
{
inline UINT PPGEBindFlagsToD3D11BindFlags(BindFlags bf)
{
    UINT d3d11_bf = 0;
    d3d11_bf |= (Enum::ToBoolean(bf & BindFlags::BIND_VERTEX_BUFFER) ? D3D11_BIND_VERTEX_BUFFER : 0);
    d3d11_bf |= (Enum::ToBoolean(bf & BindFlags::BIND_INDEX_BUFFER) ? D3D11_BIND_INDEX_BUFFER : 0);
    d3d11_bf |= (Enum::ToBoolean(bf & BindFlags::BIND_CONSTANT_BUFFER) ? D3D11_BIND_CONSTANT_BUFFER : 0);
    d3d11_bf |= (Enum::ToBoolean(bf & BindFlags::BIND_SHADER_RESOURCE) ? D3D11_BIND_SHADER_RESOURCE : 0);
    d3d11_bf |= (Enum::ToBoolean(bf & BindFlags::BIND_STREAM_OUTPUT) ? D3D11_BIND_STREAM_OUTPUT : 0);
    d3d11_bf |= (Enum::ToBoolean(bf & BindFlags::BIND_RENDER_TARGET) ? D3D11_BIND_RENDER_TARGET : 0);
    d3d11_bf |= (Enum::ToBoolean(bf & BindFlags::BIND_DEPTH_STENCIL) ? D3D11_BIND_DEPTH_STENCIL : 0);
    d3d11_bf |= (Enum::ToBoolean(bf & BindFlags::BIND_UNORDERED_ACCESS) ? D3D11_BIND_UNORDERED_ACCESS : 0);
    return d3d11_bf;
}

inline D3D11_USAGE PPGEUsageTypeToD3D11UsageType(UsageType usage)
{
    switch (usage)
    {
    case UsageType::USAGE_IMMUTABLE:
        return D3D11_USAGE_IMMUTABLE;
    case UsageType::USAGE_DYNAMIC:
        return D3D11_USAGE_DYNAMIC;
    case UsageType::USAGE_STAGING:
        return D3D11_USAGE_STAGING;
    case UsageType::USAGE_DEFAULT:
        return D3D11_USAGE_DEFAULT;
    default:
        PPGE_WARN("PPGE Usage type is unknown");
    }
    return D3D11_USAGE_DEFAULT;
}

inline UINT PPGECPUAccessFlagsToD3D11CPUAccessFlags(CPUAccessFlags cpu_af)
{
    UINT d3d11_cpu_af = 0;
    d3d11_cpu_af |= (Enum::ToBoolean(cpu_af & CPUAccessFlags::CPU_ACCESS_READ) ? D3D11_CPU_ACCESS_READ : 0);
    d3d11_cpu_af |= (Enum::ToBoolean(cpu_af & CPUAccessFlags::CPU_ACCESS_WRITE) ? D3D11_CPU_ACCESS_WRITE : 0);
    return d3d11_cpu_af;
}

inline D3D11_MAP PPGEMapTypeToD3D11MapType(MapType map_type, MapFlags map_flags)
{
    switch (map_type)
    {
    case MapType::MAP_READ:
        return D3D11_MAP_READ;
    case MapType::MAP_WRITE:
        if (Enum::ToBoolean(map_flags & MapFlags::MAP_DISCARD))
            return D3D11_MAP_WRITE_DISCARD;
        else if (Enum::ToBoolean(map_flags & MapFlags::MAP_NO_OVERWRITE))
            return D3D11_MAP_WRITE_NO_OVERWRITE;
        return D3D11_MAP_WRITE;
    case MapType::MAP_READ_WRITE:
        return D3D11_MAP_READ_WRITE;
    default:
        PPGE_ASSERT(false, "Unknown map type.");
        return static_cast<D3D11_MAP>(0);
    }
}

inline UINT PPGEMapFlagsToD3D11MapFlags(MapFlags map_flags)
{
    UINT d3d11_map_flags = 0;
    d3d11_map_flags |= (Enum::ToBoolean(map_flags & MapFlags::MAP_DO_NOT_WAIT) ? D3D11_MAP_FLAG_DO_NOT_WAIT : 0);
    return d3d11_map_flags;
}

inline DXGI_FORMAT PPGETextureFormatToD3D11TextureFormat(TextureFormatType texture_format_type)
{
    switch (texture_format_type)
    {
    case TextureFormatType::TEXTURE_FORMAT_UNKNOWN:
        return DXGI_FORMAT_UNKNOWN;
    case TextureFormatType::TEXTURE_FORMAT_R32G32B32A32_TYPELESS:
        return DXGI_FORMAT_R32G32B32A32_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_R32G32B32A32_FLOAT:
        return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case TextureFormatType::TEXTURE_FORMAT_R32G32B32A32_UINT:
        return DXGI_FORMAT_R32G32B32A32_UINT;
    case TextureFormatType::TEXTURE_FORMAT_R32G32B32A32_SINT:
        return DXGI_FORMAT_R32G32B32A32_SINT;
    case TextureFormatType::TEXTURE_FORMAT_R32G32B32_TYPELESS:
        return DXGI_FORMAT_R32G32B32_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_R32G32B32_FLOAT:
        return DXGI_FORMAT_R32G32B32_FLOAT;
    case TextureFormatType::TEXTURE_FORMAT_R32G32B32_UINT:
        return DXGI_FORMAT_R32G32B32_UINT;
    case TextureFormatType::TEXTURE_FORMAT_R32G32B32_SINT:
        return DXGI_FORMAT_R32G32B32_SINT;
    case TextureFormatType::TEXTURE_FORMAT_R16G16B16A16_TYPELESS:
        return DXGI_FORMAT_R16G16B16A16_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_R16G16B16A16_FLOAT:
        return DXGI_FORMAT_R16G16B16A16_FLOAT;
    case TextureFormatType::TEXTURE_FORMAT_R16G16B16A16_UNORM:
        return DXGI_FORMAT_R16G16B16A16_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_R16G16B16A16_UINT:
        return DXGI_FORMAT_R16G16B16A16_UINT;
    case TextureFormatType::TEXTURE_FORMAT_R16G16B16A16_SNORM:
        return DXGI_FORMAT_R16G16B16A16_SNORM;
    case TextureFormatType::TEXTURE_FORMAT_R16G16B16A16_SINT:
        return DXGI_FORMAT_R16G16B16A16_SINT;
    case TextureFormatType::TEXTURE_FORMAT_R32G32_TYPELESS:
        return DXGI_FORMAT_R32G32_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_R32G32_FLOAT:
        return DXGI_FORMAT_R32G32_FLOAT;
    case TextureFormatType::TEXTURE_FORMAT_R32G32_UINT:
        return DXGI_FORMAT_R32G32_UINT;
    case TextureFormatType::TEXTURE_FORMAT_R32G32_SINT:
        return DXGI_FORMAT_R32G32_SINT;
    case TextureFormatType::TEXTURE_FORMAT_R32G8X24_TYPELESS:
        return DXGI_FORMAT_R32G8X24_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_D32_FLOAT_S8X24_UINT:
        return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
    case TextureFormatType::TEXTURE_FORMAT_R32_FLOAT_X8X24_TYPELESS:
        return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_X32_TYPELESS_G8X24_UINT:
        return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
    case TextureFormatType::TEXTURE_FORMAT_R10G10B10A2_TYPELESS:
        return DXGI_FORMAT_R10G10B10A2_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_R10G10B10A2_UNORM:
        return DXGI_FORMAT_R10G10B10A2_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_R10G10B10A2_UINT:
        return DXGI_FORMAT_R10G10B10A2_UINT;
    case TextureFormatType::TEXTURE_FORMAT_R11G11B10_FLOAT:
        return DXGI_FORMAT_R11G11B10_FLOAT;
    case TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_TYPELESS:
        return DXGI_FORMAT_R8G8B8A8_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_UNORM:
        return DXGI_FORMAT_R8G8B8A8_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_UNORM_SRGB:
        return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    case TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_UINT:
        return DXGI_FORMAT_R8G8B8A8_UINT;
    case TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_SNORM:
        return DXGI_FORMAT_R8G8B8A8_SNORM;
    case TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_SINT:
        return DXGI_FORMAT_R8G8B8A8_SINT;
    case TextureFormatType::TEXTURE_FORMAT_R16G16_TYPELESS:
        return DXGI_FORMAT_R16G16_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_R16G16_FLOAT:
        return DXGI_FORMAT_R16G16_FLOAT;
    case TextureFormatType::TEXTURE_FORMAT_R16G16_UNORM:
        return DXGI_FORMAT_R16G16_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_R16G16_UINT:
        return DXGI_FORMAT_R16G16_UINT;
    case TextureFormatType::TEXTURE_FORMAT_R16G16_SNORM:
        return DXGI_FORMAT_R16G16_SNORM;
    case TextureFormatType::TEXTURE_FORMAT_R16G16_SINT:
        return DXGI_FORMAT_R16G16_SINT;
    case TextureFormatType::TEXTURE_FORMAT_R32_TYPELESS:
        return DXGI_FORMAT_R32_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_D32_FLOAT:
        return DXGI_FORMAT_D32_FLOAT;
    case TextureFormatType::TEXTURE_FORMAT_R32_FLOAT:
        return DXGI_FORMAT_R32_FLOAT;
    case TextureFormatType::TEXTURE_FORMAT_R32_UINT:
        return DXGI_FORMAT_R32_UINT;
    case TextureFormatType::TEXTURE_FORMAT_R32_SINT:
        return DXGI_FORMAT_R32_SINT;
    case TextureFormatType::TEXTURE_FORMAT_R24G8_TYPELESS:
        return DXGI_FORMAT_R24G8_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_D24_UNORM_S8_UINT:
        return DXGI_FORMAT_D24_UNORM_S8_UINT;
    case TextureFormatType::TEXTURE_FORMAT_R24_UNORM_X8_TYPELESS:
        return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_X24_TYPELESS_G8_UINT:
        return DXGI_FORMAT_X24_TYPELESS_G8_UINT;
    case TextureFormatType::TEXTURE_FORMAT_R8G8_TYPELESS:
        return DXGI_FORMAT_R8G8_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_R8G8_UNORM:
        return DXGI_FORMAT_R8G8_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_R8G8_UINT:
        return DXGI_FORMAT_R8G8_UINT;
    case TextureFormatType::TEXTURE_FORMAT_R8G8_SNORM:
        return DXGI_FORMAT_R8G8_SNORM;
    case TextureFormatType::TEXTURE_FORMAT_R8G8_SINT:
        return DXGI_FORMAT_R8G8_SINT;
    case TextureFormatType::TEXTURE_FORMAT_R16_TYPELESS:
        return DXGI_FORMAT_R16_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_R16_FLOAT:
        return DXGI_FORMAT_R16_FLOAT;
    case TextureFormatType::TEXTURE_FORMAT_D16_UNORM:
        return DXGI_FORMAT_D16_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_R16_UNORM:
        return DXGI_FORMAT_R16_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_R16_UINT:
        return DXGI_FORMAT_R16_UINT;
    case TextureFormatType::TEXTURE_FORMAT_R16_SNORM:
        return DXGI_FORMAT_R16_SNORM;
    case TextureFormatType::TEXTURE_FORMAT_R16_SINT:
        return DXGI_FORMAT_R16_SINT;
    case TextureFormatType::TEXTURE_FORMAT_R8_TYPELESS:
        return DXGI_FORMAT_R8_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_R8_UNORM:
        return DXGI_FORMAT_R8_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_R8_UINT:
        return DXGI_FORMAT_R8_UINT;
    case TextureFormatType::TEXTURE_FORMAT_R8_SNORM:
        return DXGI_FORMAT_R8_SNORM;
    case TextureFormatType::TEXTURE_FORMAT_R8_SINT:
        return DXGI_FORMAT_R8_SINT;
    case TextureFormatType::TEXTURE_FORMAT_A8_UNORM:
        return DXGI_FORMAT_A8_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_R1_UNORM:
        return DXGI_FORMAT_R1_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_R9G9B9E5_SHAREDEXP:
        return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
    case TextureFormatType::TEXTURE_FORMAT_R8G8_B8G8_UNORM:
        return DXGI_FORMAT_R8G8_B8G8_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_G8R8_G8B8_UNORM:
        return DXGI_FORMAT_G8R8_G8B8_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_BC1_TYPELESS:
        return DXGI_FORMAT_BC1_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_BC1_UNORM:
        return DXGI_FORMAT_BC1_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_BC1_UNORM_SRGB:
        return DXGI_FORMAT_BC1_UNORM_SRGB;
    case TextureFormatType::TEXTURE_FORMAT_BC2_TYPELESS:
        return DXGI_FORMAT_BC2_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_BC2_UNORM:
        return DXGI_FORMAT_BC2_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_BC2_UNORM_SRGB:
        return DXGI_FORMAT_BC2_UNORM_SRGB;
    case TextureFormatType::TEXTURE_FORMAT_BC3_TYPELESS:
        return DXGI_FORMAT_BC3_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_BC3_UNORM:
        return DXGI_FORMAT_BC3_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_BC3_UNORM_SRGB:
        return DXGI_FORMAT_BC3_UNORM_SRGB;
    case TextureFormatType::TEXTURE_FORMAT_BC4_TYPELESS:
        return DXGI_FORMAT_BC4_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_BC4_UNORM:
        return DXGI_FORMAT_BC4_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_BC4_SNORM:
        return DXGI_FORMAT_BC4_SNORM;
    case TextureFormatType::TEXTURE_FORMAT_BC5_TYPELESS:
        return DXGI_FORMAT_BC5_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_BC5_UNORM:
        return DXGI_FORMAT_BC5_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_BC5_SNORM:
        return DXGI_FORMAT_BC5_SNORM;
    case TextureFormatType::TEXTURE_FORMAT_B5G6R5_UNORM:
        return DXGI_FORMAT_B5G6R5_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_B5G5R5A1_UNORM:
        return DXGI_FORMAT_B5G5R5A1_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_B8G8R8A8_UNORM:
        return DXGI_FORMAT_B8G8R8A8_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_B8G8R8X8_UNORM:
        return DXGI_FORMAT_B8G8R8A8_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
        return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_B8G8R8A8_TYPELESS:
        return DXGI_FORMAT_B8G8R8A8_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_B8G8R8A8_UNORM_SRGB:
        return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    case TextureFormatType::TEXTURE_FORMAT_B8G8R8X8_TYPELESS:
        return DXGI_FORMAT_B8G8R8X8_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_B8G8R8X8_UNORM_SRGB:
        return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
    case TextureFormatType::TEXTURE_FORMAT_BC6H_TYPELESS:
        return DXGI_FORMAT_BC6H_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_BC6H_UF16:
        return DXGI_FORMAT_BC6H_UF16;
    case TextureFormatType::TEXTURE_FORMAT_BC6H_SF16:
        return DXGI_FORMAT_BC6H_SF16;
    case TextureFormatType::TEXTURE_FORMAT_BC7_TYPELESS:
        return DXGI_FORMAT_BC7_TYPELESS;
    case TextureFormatType::TEXTURE_FORMAT_BC7_UNORM:
        return DXGI_FORMAT_BC7_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_BC7_UNORM_SRGB:
        return DXGI_FORMAT_BC7_UNORM_SRGB;
    case TextureFormatType::TEXTURE_FORMAT_AYUV:
        return DXGI_FORMAT_AYUV;
    case TextureFormatType::TEXTURE_FORMAT_Y410:
        return DXGI_FORMAT_Y410;
    case TextureFormatType::TEXTURE_FORMAT_Y416:
        return DXGI_FORMAT_Y416;
    case TextureFormatType::TEXTURE_FORMAT_NV12:
        return DXGI_FORMAT_NV12;
    case TextureFormatType::TEXTURE_FORMAT_P010:
        return DXGI_FORMAT_P010;
    case TextureFormatType::TEXTURE_FORMAT_P016:
        return DXGI_FORMAT_P016;
    case TextureFormatType::TEXTURE_FORMAT_420_OPAQUE:
        return DXGI_FORMAT_420_OPAQUE;
    case TextureFormatType::TEXTURE_FORMAT_YUY2:
        return DXGI_FORMAT_YUY2;
    case TextureFormatType::TEXTURE_FORMAT_Y210:
        return DXGI_FORMAT_Y210;
    case TextureFormatType::TEXTURE_FORMAT_Y216:
        return DXGI_FORMAT_Y216;
    case TextureFormatType::TEXTURE_FORMAT_NV11:
        return DXGI_FORMAT_NV11;
    case TextureFormatType::TEXTURE_FORMAT_AI44:
        return DXGI_FORMAT_AI44;
    case TextureFormatType::TEXTURE_FORMAT_IA44:
        return DXGI_FORMAT_IA44;
    case TextureFormatType::TEXTURE_FORMAT_P8:
        return DXGI_FORMAT_P8;
    case TextureFormatType::TEXTURE_FORMAT_A8P8:
        return DXGI_FORMAT_A8P8;
    case TextureFormatType::TEXTURE_FORMAT_B4G4R4A4_UNORM:
        return DXGI_FORMAT_B4G4R4A4_UNORM;
    case TextureFormatType::TEXTURE_FORMAT_P208:
        return DXGI_FORMAT_P208;
    case TextureFormatType::TEXTURE_FORMAT_V208:
        return DXGI_FORMAT_V208;
    case TextureFormatType::TEXTURE_FORMAT_V408:
        return DXGI_FORMAT_V408;
    default:
        PPGE_ASSERT(false, "Cannot convert PPGE Texture Format to DXGI Format. PPGE Texture Format is unknown.");
        return DXGI_FORMAT_UNKNOWN;
    }
}

inline UINT PPGESwapChainUsageFlagsToD3D11SwapChainUsageFlags(SwapChainUsageFlags usage_flags)
{
    UINT d3d11_usage_flags = 0;
    d3d11_usage_flags |= (Enum::ToBoolean(usage_flags & SwapChainUsageFlags::SWAP_CHAIN_USAGE_RENDER_TARGET_OUTPUT)
                              ? DXGI_USAGE_RENDER_TARGET_OUTPUT
                              : 0);
    d3d11_usage_flags |= (Enum::ToBoolean(usage_flags & SwapChainUsageFlags::SWAP_CHAIN_USAGE_SHADER_INPUT)
                              ? DXGI_USAGE_SHADER_INPUT
                              : 0);
    d3d11_usage_flags |= (Enum::ToBoolean(usage_flags & SwapChainUsageFlags::SWAP_CHAIN_USAGE_BACK_BUFFER)
                              ? DXGI_USAGE_BACK_BUFFER
                              : 0);
    return d3d11_usage_flags;
}
} // namespace PPGE