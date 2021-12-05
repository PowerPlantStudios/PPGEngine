#pragma once
#include "PPGEpch.h"

#include "core/defines.h"

namespace PPGE
{
PPGE_HANDLE(TextureHandle, PPGE_RENDERER_MAX_TEXTURES);
PPGE_HANDLE(VertexBufferHandle, PPGE_RENDERER_MAX_VERTEX_BUFFERS);
PPGE_HANDLE(IndexBufferHandle, PPGE_RENDERER_MAX_INDEX_BUFFERS);
PPGE_HANDLE(ProgramHandle, PPGE_RENDERER_MAX_PROGRAMS);
PPGE_HANDLE(UniformHandle, PPGE_RENDERER_MAX_UNIFORMS);

template <typename HandleType> class HandleAllocator
{
    HandleAllocator() : m_available_handles(HandleType::GetMaxIdx())
    {
    }

  private:
    typedef uint16_t Idx_t;
    std::vector<Idx> m_available_handles;
};

} // namespace PPGE