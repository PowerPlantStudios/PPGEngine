#pragma once
#include "PPGEpch.h"

#include "core/defines.h"

namespace PPGE
{
inline constexpr uint16_t Invalid_Handle = HANDLE_MAX;

PPGE_HANDLE(TextureHandle);
PPGE_HANDLE(VertexBufferHandle);
PPGE_HANDLE(IndexBufferHandle);
PPGE_HANDLE(ShaderHandle);
PPGE_HANDLE(EffectHandle);

} // namespace PPGE