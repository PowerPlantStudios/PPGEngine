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
inline constexpr const char* Color_Buffer_Resource = "ColorBuffer";
inline constexpr const RenderPassTextureResourceDesc Color_Buffer_Resource_Desc{
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