#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/ppge_enum.h"
#include "rhi/rhi_types.h"
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

inline BindFlags D3D11BindFlagsToPPGEBindFlags(UINT d3d11_bf)
{
    BindFlags bf = BindFlags::BIND_NONE;
    bf |= (d3d11_bf & D3D11_BIND_VERTEX_BUFFER ? BindFlags::BIND_VERTEX_BUFFER : BindFlags::BIND_NONE);
    bf |= (d3d11_bf & D3D11_BIND_INDEX_BUFFER ? BindFlags::BIND_INDEX_BUFFER : BindFlags::BIND_NONE);
    bf |= (d3d11_bf & D3D11_BIND_CONSTANT_BUFFER ? BindFlags::BIND_CONSTANT_BUFFER : BindFlags::BIND_NONE);
    bf |= (d3d11_bf & D3D11_BIND_SHADER_RESOURCE ? BindFlags::BIND_SHADER_RESOURCE : BindFlags::BIND_NONE);
    bf |= (d3d11_bf & D3D11_BIND_STREAM_OUTPUT ? BindFlags::BIND_STREAM_OUTPUT : BindFlags::BIND_NONE);
    bf |= (d3d11_bf & D3D11_BIND_RENDER_TARGET ? BindFlags::BIND_RENDER_TARGET : BindFlags::BIND_NONE);
    bf |= (d3d11_bf & D3D11_BIND_DEPTH_STENCIL ? BindFlags::BIND_DEPTH_STENCIL : BindFlags::BIND_NONE);
    bf |= (d3d11_bf & D3D11_BIND_UNORDERED_ACCESS ? BindFlags::BIND_UNORDERED_ACCESS : BindFlags::BIND_NONE);
    return bf;
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

inline UsageType D3D11UsageTypeToPPGEUsageType(D3D11_USAGE d3d11_usage)
{
    switch (d3d11_usage)
    {
    case D3D11_USAGE_IMMUTABLE:
        return UsageType::USAGE_IMMUTABLE;
    case D3D11_USAGE_DYNAMIC:
        return UsageType::USAGE_DYNAMIC;
    case D3D11_USAGE_STAGING:
        return UsageType::USAGE_STAGING;
    case D3D11_USAGE_DEFAULT:
        return UsageType::USAGE_DEFAULT;
    default:
        PPGE_WARN("D3D11 Usage type is unknown");
    }
    return UsageType::USAGE_DEFAULT;
}

inline UINT PPGECPUAccessFlagsToD3D11CPUAccessFlags(CPUAccessFlags cpu_af)
{
    UINT d3d11_cpu_af = 0;
    d3d11_cpu_af |= (Enum::ToBoolean(cpu_af & CPUAccessFlags::CPU_ACCESS_READ) ? D3D11_CPU_ACCESS_READ : 0);
    d3d11_cpu_af |= (Enum::ToBoolean(cpu_af & CPUAccessFlags::CPU_ACCESS_WRITE) ? D3D11_CPU_ACCESS_WRITE : 0);
    return d3d11_cpu_af;
}

inline CPUAccessFlags D3D11CPUAccessFlagsToPPGECPUAccessFlags(UINT d3d11_cpu_af)
{
    CPUAccessFlags cpu_af = CPUAccessFlags::CPU_ACCESS_NONE;
    cpu_af |=
        (d3d11_cpu_af & D3D11_CPU_ACCESS_READ ? CPUAccessFlags::CPU_ACCESS_READ : CPUAccessFlags::CPU_ACCESS_NONE);
    cpu_af |=
        (d3d11_cpu_af & D3D11_CPU_ACCESS_WRITE ? CPUAccessFlags::CPU_ACCESS_WRITE : CPUAccessFlags::CPU_ACCESS_NONE);
    return cpu_af;
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
        return DXGI_FORMAT_B8G8R8X8_UNORM;
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

inline TextureFormatType D3D11TextureFormatToPPGETextureFormat(DXGI_FORMAT d3d11_texture_format_type)
{
    switch (d3d11_texture_format_type)
    {
    case DXGI_FORMAT_UNKNOWN:
        return TextureFormatType::TEXTURE_FORMAT_UNKNOWN;
    case DXGI_FORMAT_R32G32B32A32_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_R32G32B32A32_TYPELESS;
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
        return TextureFormatType::TEXTURE_FORMAT_R32G32B32A32_FLOAT;
    case DXGI_FORMAT_R32G32B32A32_UINT:
        return TextureFormatType::TEXTURE_FORMAT_R32G32B32A32_UINT;
    case DXGI_FORMAT_R32G32B32A32_SINT:
        return TextureFormatType::TEXTURE_FORMAT_R32G32B32A32_SINT;
    case DXGI_FORMAT_R32G32B32_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_R32G32B32_TYPELESS;
    case DXGI_FORMAT_R32G32B32_FLOAT:
        return TextureFormatType::TEXTURE_FORMAT_R32G32B32_FLOAT;
    case DXGI_FORMAT_R32G32B32_UINT:
        return TextureFormatType::TEXTURE_FORMAT_R32G32B32_UINT;
    case DXGI_FORMAT_R32G32B32_SINT:
        return TextureFormatType::TEXTURE_FORMAT_R32G32B32_SINT;
    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_R16G16B16A16_TYPELESS;
    case DXGI_FORMAT_R16G16B16A16_FLOAT:
        return TextureFormatType::TEXTURE_FORMAT_R16G16B16A16_FLOAT;
    case DXGI_FORMAT_R16G16B16A16_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_R16G16B16A16_UNORM;
    case DXGI_FORMAT_R16G16B16A16_UINT:
        return TextureFormatType::TEXTURE_FORMAT_R16G16B16A16_UINT;
    case DXGI_FORMAT_R16G16B16A16_SNORM:
        return TextureFormatType::TEXTURE_FORMAT_R16G16B16A16_SNORM;
    case DXGI_FORMAT_R16G16B16A16_SINT:
        return TextureFormatType::TEXTURE_FORMAT_R16G16B16A16_SINT;
    case DXGI_FORMAT_R32G32_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_R32G32_TYPELESS;
    case DXGI_FORMAT_R32G32_FLOAT:
        return TextureFormatType::TEXTURE_FORMAT_R32G32_FLOAT;
    case DXGI_FORMAT_R32G32_UINT:
        return TextureFormatType::TEXTURE_FORMAT_R32G32_UINT;
    case DXGI_FORMAT_R32G32_SINT:
        return TextureFormatType::TEXTURE_FORMAT_R32G32_SINT;
    case DXGI_FORMAT_R32G8X24_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_R32G8X24_TYPELESS;
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
        return TextureFormatType::TEXTURE_FORMAT_D32_FLOAT_S8X24_UINT;
    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_R32_FLOAT_X8X24_TYPELESS;
    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
        return TextureFormatType::TEXTURE_FORMAT_X32_TYPELESS_G8X24_UINT;
    case DXGI_FORMAT_R10G10B10A2_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_R10G10B10A2_TYPELESS;
    case DXGI_FORMAT_R10G10B10A2_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_R10G10B10A2_UNORM;
    case DXGI_FORMAT_R10G10B10A2_UINT:
        return TextureFormatType::TEXTURE_FORMAT_R10G10B10A2_UINT;
    case DXGI_FORMAT_R11G11B10_FLOAT:
        return TextureFormatType::TEXTURE_FORMAT_R11G11B10_FLOAT;
    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_TYPELESS;
    case DXGI_FORMAT_R8G8B8A8_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_UNORM;
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        return TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_UNORM_SRGB;
    case DXGI_FORMAT_R8G8B8A8_UINT:
        return TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_UINT;
    case DXGI_FORMAT_R8G8B8A8_SNORM:
        return TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_SNORM;
    case DXGI_FORMAT_R8G8B8A8_SINT:
        return TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_SINT;
    case DXGI_FORMAT_R16G16_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_R16G16_TYPELESS;
    case DXGI_FORMAT_R16G16_FLOAT:
        return TextureFormatType::TEXTURE_FORMAT_R16G16_FLOAT;
    case DXGI_FORMAT_R16G16_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_R16G16_UNORM;
    case DXGI_FORMAT_R16G16_UINT:
        return TextureFormatType::TEXTURE_FORMAT_R16G16_UINT;
    case DXGI_FORMAT_R16G16_SNORM:
        return TextureFormatType::TEXTURE_FORMAT_R16G16_SNORM;
    case DXGI_FORMAT_R16G16_SINT:
        return TextureFormatType::TEXTURE_FORMAT_R16G16_SINT;
    case DXGI_FORMAT_R32_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_R32_TYPELESS;
    case DXGI_FORMAT_D32_FLOAT:
        return TextureFormatType::TEXTURE_FORMAT_D32_FLOAT;
    case DXGI_FORMAT_R32_FLOAT:
        return TextureFormatType::TEXTURE_FORMAT_R32_FLOAT;
    case DXGI_FORMAT_R32_UINT:
        return TextureFormatType::TEXTURE_FORMAT_R32_UINT;
    case DXGI_FORMAT_R32_SINT:
        return TextureFormatType::TEXTURE_FORMAT_R32_SINT;
    case DXGI_FORMAT_R24G8_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_R24G8_TYPELESS;
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
        return TextureFormatType::TEXTURE_FORMAT_D24_UNORM_S8_UINT;
    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_R24_UNORM_X8_TYPELESS;
    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
        return TextureFormatType::TEXTURE_FORMAT_X24_TYPELESS_G8_UINT;
    case DXGI_FORMAT_R8G8_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_R8G8_TYPELESS;
    case DXGI_FORMAT_R8G8_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_R8G8_UNORM;
    case DXGI_FORMAT_R8G8_UINT:
        return TextureFormatType::TEXTURE_FORMAT_R8G8_UINT;
    case DXGI_FORMAT_R8G8_SNORM:
        return TextureFormatType::TEXTURE_FORMAT_R8G8_SNORM;
    case DXGI_FORMAT_R8G8_SINT:
        return TextureFormatType::TEXTURE_FORMAT_R8G8_SINT;
    case DXGI_FORMAT_R16_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_R16_TYPELESS;
    case DXGI_FORMAT_R16_FLOAT:
        return TextureFormatType::TEXTURE_FORMAT_R16_FLOAT;
    case DXGI_FORMAT_D16_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_D16_UNORM;
    case DXGI_FORMAT_R16_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_R16_UNORM;
    case DXGI_FORMAT_R16_UINT:
        return TextureFormatType::TEXTURE_FORMAT_R16_UINT;
    case DXGI_FORMAT_R16_SNORM:
        return TextureFormatType::TEXTURE_FORMAT_R16_SNORM;
    case DXGI_FORMAT_R16_SINT:
        return TextureFormatType::TEXTURE_FORMAT_R16_SINT;
    case DXGI_FORMAT_R8_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_R8_TYPELESS;
    case DXGI_FORMAT_R8_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_R8_UNORM;
    case DXGI_FORMAT_R8_UINT:
        return TextureFormatType::TEXTURE_FORMAT_R8_UINT;
    case DXGI_FORMAT_R8_SNORM:
        return TextureFormatType::TEXTURE_FORMAT_R8_SNORM;
    case DXGI_FORMAT_R8_SINT:
        return TextureFormatType::TEXTURE_FORMAT_R8_SINT;
    case DXGI_FORMAT_A8_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_A8_UNORM;
    case DXGI_FORMAT_R1_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_R1_UNORM;
    case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
        return TextureFormatType::TEXTURE_FORMAT_R9G9B9E5_SHAREDEXP;
    case DXGI_FORMAT_R8G8_B8G8_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_R8G8_B8G8_UNORM;
    case DXGI_FORMAT_G8R8_G8B8_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_G8R8_G8B8_UNORM;
    case DXGI_FORMAT_BC1_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_BC1_TYPELESS;
    case DXGI_FORMAT_BC1_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_BC1_UNORM;
    case DXGI_FORMAT_BC1_UNORM_SRGB:
        return TextureFormatType::TEXTURE_FORMAT_BC1_UNORM_SRGB;
    case DXGI_FORMAT_BC2_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_BC2_TYPELESS;
    case DXGI_FORMAT_BC2_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_BC2_UNORM;
    case DXGI_FORMAT_BC2_UNORM_SRGB:
        return TextureFormatType::TEXTURE_FORMAT_BC2_UNORM_SRGB;
    case DXGI_FORMAT_BC3_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_BC3_TYPELESS;
    case DXGI_FORMAT_BC3_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_BC3_UNORM;
    case DXGI_FORMAT_BC3_UNORM_SRGB:
        return TextureFormatType::TEXTURE_FORMAT_BC3_UNORM_SRGB;
    case DXGI_FORMAT_BC4_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_BC4_TYPELESS;
    case DXGI_FORMAT_BC4_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_BC4_UNORM;
    case DXGI_FORMAT_BC4_SNORM:
        return TextureFormatType::TEXTURE_FORMAT_BC4_SNORM;
    case DXGI_FORMAT_BC5_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_BC5_TYPELESS;
    case DXGI_FORMAT_BC5_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_BC5_UNORM;
    case DXGI_FORMAT_BC5_SNORM:
        return TextureFormatType::TEXTURE_FORMAT_BC5_SNORM;
    case DXGI_FORMAT_B5G6R5_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_B5G6R5_UNORM;
    case DXGI_FORMAT_B5G5R5A1_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_B5G5R5A1_UNORM;
    case DXGI_FORMAT_B8G8R8A8_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_B8G8R8A8_UNORM;
    case DXGI_FORMAT_B8G8R8X8_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_B8G8R8X8_UNORM;
    case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
    case DXGI_FORMAT_B8G8R8A8_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_B8G8R8A8_TYPELESS;
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        return TextureFormatType::TEXTURE_FORMAT_B8G8R8A8_UNORM_SRGB;
    case DXGI_FORMAT_B8G8R8X8_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_B8G8R8X8_TYPELESS;
    case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
        return TextureFormatType::TEXTURE_FORMAT_B8G8R8X8_UNORM_SRGB;
    case DXGI_FORMAT_BC6H_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_BC6H_TYPELESS;
    case DXGI_FORMAT_BC6H_UF16:
        return TextureFormatType::TEXTURE_FORMAT_BC6H_UF16;
    case DXGI_FORMAT_BC6H_SF16:
        return TextureFormatType::TEXTURE_FORMAT_BC6H_SF16;
    case DXGI_FORMAT_BC7_TYPELESS:
        return TextureFormatType::TEXTURE_FORMAT_BC7_TYPELESS;
    case DXGI_FORMAT_BC7_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_BC7_UNORM;
    case DXGI_FORMAT_BC7_UNORM_SRGB:
        return TextureFormatType::TEXTURE_FORMAT_BC7_UNORM_SRGB;
    case DXGI_FORMAT_AYUV:
        return TextureFormatType::TEXTURE_FORMAT_AYUV;
    case DXGI_FORMAT_Y410:
        return TextureFormatType::TEXTURE_FORMAT_Y410;
    case DXGI_FORMAT_Y416:
        return TextureFormatType::TEXTURE_FORMAT_Y416;
    case DXGI_FORMAT_NV12:
        return TextureFormatType::TEXTURE_FORMAT_NV12;
    case DXGI_FORMAT_P010:
        return TextureFormatType::TEXTURE_FORMAT_P010;
    case DXGI_FORMAT_P016:
        return TextureFormatType::TEXTURE_FORMAT_P016;
    case DXGI_FORMAT_420_OPAQUE:
        return TextureFormatType::TEXTURE_FORMAT_420_OPAQUE;
    case DXGI_FORMAT_YUY2:
        return TextureFormatType::TEXTURE_FORMAT_YUY2;
    case DXGI_FORMAT_Y210:
        return TextureFormatType::TEXTURE_FORMAT_Y210;
    case DXGI_FORMAT_Y216:
        return TextureFormatType::TEXTURE_FORMAT_Y216;
    case DXGI_FORMAT_NV11:
        return TextureFormatType::TEXTURE_FORMAT_NV11;
    case DXGI_FORMAT_AI44:
        return TextureFormatType::TEXTURE_FORMAT_AI44;
    case DXGI_FORMAT_IA44:
        return TextureFormatType::TEXTURE_FORMAT_IA44;
    case DXGI_FORMAT_P8:
        return TextureFormatType::TEXTURE_FORMAT_P8;
    case DXGI_FORMAT_A8P8:
        return TextureFormatType::TEXTURE_FORMAT_A8P8;
    case DXGI_FORMAT_B4G4R4A4_UNORM:
        return TextureFormatType::TEXTURE_FORMAT_B4G4R4A4_UNORM;
    case DXGI_FORMAT_P208:
        return TextureFormatType::TEXTURE_FORMAT_P208;
    case DXGI_FORMAT_V208:
        return TextureFormatType::TEXTURE_FORMAT_V208;
    case DXGI_FORMAT_V408:
        return TextureFormatType::TEXTURE_FORMAT_V408;
    default:
        PPGE_ASSERT(false, "Cannot convert DXGI Format to PPGE Texture Format. DXGI Format is unknown.");
        return TextureFormatType::TEXTURE_FORMAT_UNKNOWN;
    }
}

inline UINT PPGESwapChainUsageFlagsToD3D11SwapChainUsageFlags(SwapChainUsageFlags usage_flags)
{
    UINT d3d11_usage_flags = 0;
    d3d11_usage_flags |= (Enum::ToBoolean(usage_flags & SwapChainUsageFlags::SWAP_CHAIN_USAGE_RENDER_TARGET_OUTPUT)
                              ? DXGI_USAGE_RENDER_TARGET_OUTPUT
                              : 0);
    d3d11_usage_flags |=
        (Enum::ToBoolean(usage_flags & SwapChainUsageFlags::SWAP_CHAIN_USAGE_SHADER_INPUT) ? DXGI_USAGE_SHADER_INPUT
                                                                                           : 0);
    d3d11_usage_flags |=
        (Enum::ToBoolean(usage_flags & SwapChainUsageFlags::SWAP_CHAIN_USAGE_BACK_BUFFER) ? DXGI_USAGE_BACK_BUFFER : 0);
    return d3d11_usage_flags;
}

inline D3D11_INPUT_CLASSIFICATION PPGEElementClassificationTypeToD3D11InputClassification(
    ElementClassificationType element_classification_type)
{
    switch (element_classification_type)
    {
    case PPGE::ElementClassificationType::ELEMENT_CLASSIFICATION_PER_VERTEX_DATA:
        return D3D11_INPUT_PER_VERTEX_DATA;
    case PPGE::ElementClassificationType::ELEMENT_CLASSIFICATION_PER_INSTANCE_DATA:
        return D3D11_INPUT_PER_INSTANCE_DATA;
    default:
        PPGE_ASSERT(false, "Unknown ElementClassificationType.");
        return (D3D11_INPUT_CLASSIFICATION)2;
    }
}

inline D3D11_BLEND PPGEBlendTypeToD3D11Blend(BlendType blend_type)
{
    switch (blend_type)
    {
    case PPGE::BlendType::BLEND_ZERO:
        return D3D11_BLEND_ZERO;
    case PPGE::BlendType::BLEND_ONE:
        return D3D11_BLEND_ONE;
    case PPGE::BlendType::BLEND_SRC_COLOR:
        return D3D11_BLEND_SRC_COLOR;
    case PPGE::BlendType::BLEND_INV_SRC_COLOR:
        return D3D11_BLEND_INV_SRC_COLOR;
    case PPGE::BlendType::BLEND_SRC_ALPHA:
        return D3D11_BLEND_SRC_ALPHA;
    case PPGE::BlendType::BLEND_INV_SRC_ALPHA:
        return D3D11_BLEND_INV_SRC_ALPHA;
    case PPGE::BlendType::BLEND_DEST_ALPHA:
        return D3D11_BLEND_DEST_ALPHA;
    case PPGE::BlendType::BLEND_INV_DEST_ALPHA:
        return D3D11_BLEND_INV_DEST_ALPHA;
    case PPGE::BlendType::BLEND_DEST_COLOR:
        return D3D11_BLEND_DEST_COLOR;
    case PPGE::BlendType::BLEND_INV_DEST_COLOR:
        return D3D11_BLEND_INV_DEST_COLOR;
    case PPGE::BlendType::BLEND_SRC_ALPHA_SAT:
        return D3D11_BLEND_SRC_ALPHA_SAT;
    case PPGE::BlendType::BLEND_BLEND_FACTOR:
        return D3D11_BLEND_BLEND_FACTOR;
    case PPGE::BlendType::BLEND_INV_BLEND_FACTOR:
        return D3D11_BLEND_INV_BLEND_FACTOR;
    case PPGE::BlendType::BLEND_SRC1_COLOR:
        return D3D11_BLEND_SRC1_COLOR;
    case PPGE::BlendType::BLEND_INV_SRC1_COLOR:
        return D3D11_BLEND_INV_SRC1_COLOR;
    case PPGE::BlendType::BLEND_SRC1_ALPHA:
        return D3D11_BLEND_SRC1_ALPHA;
    case PPGE::BlendType::BLEND_INV_SRC1_ALPHA:
        return D3D11_BLEND_INV_SRC1_ALPHA;
    case PPGE::BlendType::BLEND_UNDEFINED:
        PPGE_ASSERT(false, "Blend type is undefined.");
        return (D3D11_BLEND)0;
    default:
        PPGE_ASSERT(false, "Blend type is unknown.");
        return (D3D11_BLEND)0;
    }
}

inline D3D11_BLEND_OP PPGEBlendOpTypeToD3D11BlendOp(BlendOpType blend_op)
{
    switch (blend_op)
    {
    case PPGE::BlendOpType::BLEND_OP_ADD:
        return D3D11_BLEND_OP_ADD;
    case PPGE::BlendOpType::BLEND_OP_SUBTRACT:
        return D3D11_BLEND_OP_SUBTRACT;
    case PPGE::BlendOpType::BLEND_OP_REV_SUBTRACT:
        return D3D11_BLEND_OP_REV_SUBTRACT;
    case PPGE::BlendOpType::BLEND_OP_MIN:
        return D3D11_BLEND_OP_MIN;
    case PPGE::BlendOpType::BLEND_OP_MAX:
        return D3D11_BLEND_OP_MAX;
    case PPGE::BlendOpType::BLEND_OP_UNDEFINED:
        PPGE_ASSERT(false, "Blend operation type is undefined.");
        return (D3D11_BLEND_OP)0;
    default:
        PPGE_ASSERT(false, "Blend operation type is unknown.");
        return (D3D11_BLEND_OP)0;
    }
}

inline UINT PPGEColorWriteEnableFlagsToD3D11ColorWriteEnableFlags(ColorWriteEnableFlags color_write_enable_flags)
{
    UINT d3d11_color_write_enable_flags = 0;
    d3d11_color_write_enable_flags |=
        (Enum::ToBoolean(color_write_enable_flags & ColorWriteEnableFlags::COLOR_WRITE_ENABLE_RED)
             ? D3D11_COLOR_WRITE_ENABLE_RED
             : 0);
    d3d11_color_write_enable_flags |=
        (Enum::ToBoolean(color_write_enable_flags & ColorWriteEnableFlags::COLOR_WRITE_ENABLE_GREEN)
             ? D3D11_COLOR_WRITE_ENABLE_GREEN
             : 0);
    d3d11_color_write_enable_flags |=
        (Enum::ToBoolean(color_write_enable_flags & ColorWriteEnableFlags::COLOR_WRITE_ENABLE_BLUE)
             ? D3D11_COLOR_WRITE_ENABLE_BLUE
             : 0);
    d3d11_color_write_enable_flags |=
        (Enum::ToBoolean(color_write_enable_flags & ColorWriteEnableFlags::COLOR_WRITE_ENABLE_ALPHA)
             ? D3D11_COLOR_WRITE_ENABLE_ALPHA
             : 0);
    return d3d11_color_write_enable_flags;
}

inline D3D11_FILL_MODE PPGEFillModeTypeToD3D11FillMode(FillModeType fill_mode)
{
    switch (fill_mode)
    {
    case PPGE::FillModeType::FILL_MODE_WIREFRAME:
        return D3D11_FILL_WIREFRAME;
    case PPGE::FillModeType::FILL_MODE_SOLID:
        return D3D11_FILL_SOLID;
    case PPGE::FillModeType::FILL_MODE_UNDEFINED:
        PPGE_ASSERT(false, "Fill mode type is undefined.");
        return (D3D11_FILL_MODE)0;
    default:
        PPGE_ASSERT(false, "Fill mode type is unknown.");
        return (D3D11_FILL_MODE)0;
    }
}

inline D3D11_CULL_MODE PPGECullModeTypeToD3D11CullMode(CullModeType cull_mode)
{
    switch (cull_mode)
    {
    case PPGE::CullModeType::CULL_MODE_NONE:
        return D3D11_CULL_NONE;
    case PPGE::CullModeType::CULL_MODE_FRONT:
        return D3D11_CULL_FRONT;
    case PPGE::CullModeType::CULL_MODE_BACK:
        return D3D11_CULL_BACK;
    case PPGE::CullModeType::CULL_MODE_UNDEFINED:
        PPGE_ASSERT(false, "Fill mode type is undefined.");
        return (D3D11_CULL_MODE)0;
    default:
        PPGE_ASSERT(false, "Fill mode type is unknown.");
        return (D3D11_CULL_MODE)0;
    }
}

inline D3D11_COMPARISON_FUNC PPGEComperisonFuncTypeToD3D11ComparisonFunc(ComperisonFuncType comparison_func)
{
    switch (comparison_func)
    {
    case PPGE::ComperisonFuncType::COMPARISON_FUNC_NEVER:
        return D3D11_COMPARISON_NEVER;
    case PPGE::ComperisonFuncType::COMPARISON_FUNC_LESS:
        return D3D11_COMPARISON_LESS;
    case PPGE::ComperisonFuncType::COMPARISON_FUNC_EQUAL:
        return D3D11_COMPARISON_EQUAL;
    case PPGE::ComperisonFuncType::COMPARISON_FUNC_LESS_EQUAL:
        return D3D11_COMPARISON_LESS_EQUAL;
    case PPGE::ComperisonFuncType::COMPARISON_FUNC_GREATER:
        return D3D11_COMPARISON_GREATER;
    case PPGE::ComperisonFuncType::COMPARISON_FUNC_NOT_EQUAL:
        return D3D11_COMPARISON_NOT_EQUAL;
    case PPGE::ComperisonFuncType::COMPARISON_FUNC_GREATER_EQUAL:
        return D3D11_COMPARISON_GREATER_EQUAL;
    case PPGE::ComperisonFuncType::COMPARISON_FUNC_ALWAYS:
        return D3D11_COMPARISON_ALWAYS;
    case PPGE::ComperisonFuncType::COMPARISON_FUNC_UNDEFINED:
        PPGE_ASSERT(false, "Comparison function type is undefined.");
        return (D3D11_COMPARISON_FUNC)0;
    default:
        PPGE_ASSERT(false, "Comparison function type is unknown.");
        return (D3D11_COMPARISON_FUNC)0;
    }
}

inline D3D11_STENCIL_OP PPGEStencilOpTypeToD3D11StencilOp(StencilOpType stencil_op)
{
    switch (stencil_op)
    {
    case PPGE::StencilOpType::STENCIL_OP_KEEP:
        return D3D11_STENCIL_OP_KEEP;
    case PPGE::StencilOpType::STENCIL_OP_ZERO:
        return D3D11_STENCIL_OP_ZERO;
    case PPGE::StencilOpType::STENCIL_OP_REPLACE:
        return D3D11_STENCIL_OP_REPLACE;
    case PPGE::StencilOpType::STENCIL_OP_INCR_SAT:
        return D3D11_STENCIL_OP_INCR_SAT;
    case PPGE::StencilOpType::STENCIL_OP_DECR_SAT:
        return D3D11_STENCIL_OP_DECR_SAT;
    case PPGE::StencilOpType::STENCIL_OP_INVERT:
        return D3D11_STENCIL_OP_INVERT;
    case PPGE::StencilOpType::STENCIL_OP_INCR:
        return D3D11_STENCIL_OP_INCR;
    case PPGE::StencilOpType::STENCIL_OP_DECR:
        return D3D11_STENCIL_OP_DECR;
    case PPGE::StencilOpType::STENCIL_OP_UNDEFINED:
        PPGE_ASSERT(false, "Stencil op type is undefined.");
        return (D3D11_STENCIL_OP)0;
    default:
        PPGE_ASSERT(false, "Stencil op type is unknown.");
        return (D3D11_STENCIL_OP)0;
    }
}

inline D3D11_PRIMITIVE_TOPOLOGY PPGEPrimitiveTopologyTypeToD3D11PrimitiveTopology(
    PrimitiveTopologyType primitive_topology)
{
    switch (primitive_topology)
    {
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_UNDEFINED:
        return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_POINTLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_LINELIST:
        return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_LINESTRIP:
        return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_TRIANGLELIST:
        return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
        return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_LINELIST_ADJ:
        return D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ:
        return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ:
        return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ:
        return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST;
    case PPGE::PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST:
        return D3D11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST;
    default:
        PPGE_ASSERT(false, "Primitive topology type is unknown.");
        return (D3D11_PRIMITIVE_TOPOLOGY)0;
    }
}
} // namespace PPGE