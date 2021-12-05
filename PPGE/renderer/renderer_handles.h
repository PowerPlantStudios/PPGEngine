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

template <typename HandleType, typename Idx_t = uint16_t> class HandleAllocator
{
    HandleAllocator() : m_available_handles(HandleType::GetMaxIdx())
    {
    }

    HandleType GetNext()
    {
        Idx_t idx = m_available_handles.back();
        m_available_handles.pop_back();
        HandleType handle{idx};
        return handle;
    }

    void Restore(HandleType handle)
    {
        Idx_t idx = handle.idx;
        m_available_handles.push_back(idx);
    }

  private:
    std::vector<Idx_t> m_available_handles;
};

} // namespace PPGE