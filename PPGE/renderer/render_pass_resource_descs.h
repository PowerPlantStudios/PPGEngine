#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/render_pass.h"
#include "rhi/buffer.h"
#include "rhi/texture.h"

namespace PPGE
{
namespace RenderPassResourceDescs
{
inline constexpr const char *Albedo_Buffer_Resource = "AlbedoBuffer";
inline constexpr const RenderPassTextureResourceDesc Albedo_Buffer_Resource_Desc{
    TextureDesc{.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D,
                .width = 0U,
                .height = 0U,
                .format_type = TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_UNORM,
                .bind_flags = (BindFlags::BIND_SHADER_RESOURCE | BindFlags::BIND_RENDER_TARGET)}};

inline constexpr const char *Normal_Buffer_Resource = "NormalBuffer";
inline constexpr const RenderPassTextureResourceDesc Normal_Buffer_Resource_Desc{
    TextureDesc{.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D,
                .width = 0U,
                .height = 0U,
                .format_type = TextureFormatType::TEXTURE_FORMAT_R16G16B16A16_FLOAT,
                .bind_flags = (BindFlags::BIND_SHADER_RESOURCE | BindFlags::BIND_RENDER_TARGET)}};

inline constexpr const char *Position_Buffer_Resource = "PositionBuffer";
inline constexpr const RenderPassTextureResourceDesc Position_Buffer_Resource_Desc{
    TextureDesc{.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D,
                .width = 0U,
                .height = 0U,
                .format_type = TextureFormatType::TEXTURE_FORMAT_R16G16B16A16_FLOAT,
                .bind_flags = (BindFlags::BIND_SHADER_RESOURCE | BindFlags::BIND_RENDER_TARGET)}};

inline constexpr const char *Material_Buffer_Resource = "MaterialBuffer";
inline constexpr const RenderPassTextureResourceDesc Material_Buffer_Resource_Desc{
    TextureDesc{.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D,
                .width = 0U,
                .height = 0U,
                .format_type = TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_UNORM,
                .bind_flags = (BindFlags::BIND_SHADER_RESOURCE | BindFlags::BIND_RENDER_TARGET)}};

inline constexpr const char *Emission_Buffer_Resource = "EmissionBuffer";
inline constexpr const RenderPassTextureResourceDesc Emission_Buffer_Resource_Desc{
    TextureDesc{.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D,
                .width = 0U,
                .height = 0U,
                .format_type = TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_UNORM,
                .bind_flags = (BindFlags::BIND_SHADER_RESOURCE | BindFlags::BIND_RENDER_TARGET)}};

inline constexpr const char* Present_Buffer_Resource = "PresentBuffer";
inline constexpr const RenderPassTextureResourceDesc Present_Buffer_Resource_Desc{
    TextureDesc{.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D,
                .width = 0U,
                .height = 0U,
                .format_type = TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_UNORM,
                .bind_flags = (BindFlags::BIND_SHADER_RESOURCE | BindFlags::BIND_RENDER_TARGET)}};

inline constexpr const char* Depth_Buffer_Resource = "DepthBuffer";
inline constexpr const RenderPassTextureResourceDesc Depth_Buffer_Resource_Desc{
    TextureDesc{.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D,
                .width = 0U,
                .height = 0U,
                .format_type = TextureFormatType::TEXTURE_FORMAT_D24_UNORM_S8_UINT,
                .bind_flags = BindFlags::BIND_DEPTH_STENCIL}};

}
} // namespace PPGE