#pragma once
#include "PPGEpch.h"

#include "core/defines.h"

namespace PPGE
{
template <typename HandleType, typename Idx_t = uint16_t> class HandleAllocator
{
  public:
    void Reset()
    {
        m_available_handles.resize(HandleType::GetMaxIdx());
        std::iota(m_available_handles.rbegin(), m_available_handles.rend(), 0);
    }

    HandleAllocator()
    {
        m_available_handles.reserve(HandleType::GetMaxIdx());
        Reset();
    }

    ~HandleAllocator() = default;

    HandleAllocator(HandleAllocator &) = delete;

    HandleType GetNext()
    {
        HandleType handle;
        if (m_available_handles.size() > 0)
        {
            handle.idx = m_available_handles.back();
            m_available_handles.pop_back();
        }
        return handle;
    }

    void Restore(HandleType handle)
    {
        if (!handle.IsValid())
            return;
        m_available_handles.push_back(handle.idx);
    }

  private:
    std::vector<Idx_t> m_available_handles;
};

PPGE_HANDLE(VertexBufferHandle, PPGE_RENDERER_MAX_VERTEX_BUFFERS)

PPGE_HANDLE(IndexBufferHandle, PPGE_RENDERER_MAX_INDEX_BUFFERS)

PPGE_HANDLE(TextureHandle, PPGE_RENDERER_MAX_TEXTURES)

PPGE_HANDLE(ProgramHandle, PPGE_RENDERER_MAX_PROGRAMS)

PPGE_HANDLE(UniformHandle, PPGE_RENDERER_MAX_UNIFORMS)
} // namespace PPGE