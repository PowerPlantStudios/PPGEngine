#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"

namespace PPGE
{
template <typename HandleType, typename Idx_t = uint16_t> class HandleAllocator
{
  public:
    void Reset()
    {
        m_handle_ref_count.clear();
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
            Idx_t idx = m_available_handles.back();
            m_available_handles.pop_back();
            m_handle_ref_count[idx] = 1ull;
            handle.idx = idx;
        }
        return handle;
    }

    void Restore(HandleType handle)
    {
        if (!handle.IsValid())
            return;

        auto search = m_handle_ref_count.find(handle.idx);
        PPGE_ASSERT(search != m_handle_ref_count.end(),
                    "Renderer resource allocator has failed to find handle reference count.");
        if (search->second == 1)
        {
            m_handle_ref_count.erase(search);
            m_available_handles.push_back(handle.idx);
            return;
        }
        search->second -= 1;
    }

    void ShareResource(HandleType handle)
    {
        if (!handle.IsValid())
            return;

        auto search = m_handle_ref_count.find(handle.idx);
        PPGE_ASSERT(search != m_handle_ref_count.end(),
                    "Renderer resource allocator has failed to find handle reference count.");
        search->second += 1;
    }

    bool HandleIsUsed(HandleType handle)
    {
        if (!handle.IsValid())
            return false;
        return m_handle_ref_count.find(handle.idx) != m_handle_ref_count.end();
    }

  private:
    std::vector<Idx_t> m_available_handles;
    std::unordered_map<Idx_t, size_t> m_handle_ref_count;
};

PPGE_HANDLE(VertexBufferHandle, PPGE_RENDERER_MAX_VERTEX_BUFFERS)

PPGE_HANDLE(VertexLayoutHandle, PPGE_RENDERER_MAX_VERTEX_LAYOUTS)

PPGE_HANDLE(IndexBufferHandle, PPGE_RENDERER_MAX_INDEX_BUFFERS)

PPGE_HANDLE(TextureHandle, PPGE_RENDERER_MAX_TEXTURES)

PPGE_HANDLE(SamplerHandle, PPGE_RENDERER_MAX_SAMPLER)

PPGE_HANDLE(ShaderHandle, PPGE_RENDERER_MAX_SHADERS)

PPGE_HANDLE(ProgramHandle, PPGE_RENDERER_MAX_PROGRAMS)

PPGE_HANDLE(UniformHandle, PPGE_RENDERER_MAX_UNIFORMS)
} // namespace PPGE