#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/descriptors.h"
#include "renderer/renderer_handles.h"
#include "system/logger_system.h"

namespace PPGE
{
template <class Handle> struct UpdatableResource
{
    Handle handle;
    Subresource subresource;
};

template <class Handle> struct BindableShaderResource
{
    Handle handle;
    ShaderResourceTarget target;
    uint8_t slot;
};

struct DrawData
{
    using UniformUpdate = UpdatableResource<UniformHandle>;

    using UniformBind = BindableShaderResource<UniformHandle>;
    using TextureBind = BindableShaderResource<TextureHandle>;
    using SamplerBind = BindableShaderResource<SamplerHandle>;

    VertexBufferHandle vb_handle;
    VertexLayoutHandle ly_handle;
    IndexBufferHandle ib_handle;
    ProgramHandle pg_handle;

    UniformUpdate un_updates[10];
    uint8_t un_update_count = 0;

    UniformBind un_binds[10];
    uint8_t un_bind_count = 0;
    TextureBind tex_binds[10];
    uint8_t tex_bind_count = 0;
    SamplerBind sp_binds[10];
    uint8_t sp_bind_count = 0;

    void PushUniformUpdate(UniformHandle handle, Subresource subresource)
    {
        PPGE_ASSERT(un_update_count < 10, "Number of per object uniform update has exceeded the maximum limit.");
        UniformUpdate &update = un_updates[un_update_count++];
        update.handle = handle;
        update.subresource = subresource;
    }

    void PushUniformBind(UniformHandle handle, ShaderResourceTarget target, uint8_t slot)
    {
        PPGE_ASSERT(un_bind_count < 10, "Number of per object uniform bind has exceeded the maximum limit.");
        UniformBind &update = un_binds[un_bind_count++];
        update.handle = handle;
        update.target = target;
        update.slot = slot;
    }

    void PushTextureBind(TextureHandle handle, ShaderResourceTarget target, uint8_t slot)
    {
        PPGE_ASSERT(tex_bind_count < 10, "Number of per object texture bind has exceeded the maximum limit.");
        TextureBind &update = tex_binds[tex_bind_count++];
        update.handle = handle;
        update.target = target;
        update.slot = slot;
    }

    void PushSamplerBind(SamplerHandle handle, ShaderResourceTarget target, uint8_t slot)
    {
        PPGE_ASSERT(sp_bind_count < 10, "Number of per object sampler bind has exceeded the maximum limit.");
        SamplerBind &update = sp_binds[sp_bind_count++];
        update.handle = handle;
        update.target = target;
        update.slot = slot;
    }

    void Reset()
    {
        un_update_count = 0;
        un_bind_count = 0;
        tex_bind_count = 0;
        sp_bind_count = 0;
    }
};

class Frame
{
  public:
    void SetVertexBufferHandle(VertexBufferHandle handle);
    void SetVertexLayoutHandle(VertexLayoutHandle handle);
    void SetIndexBufferHandle(IndexBufferHandle handle);
    void UpdateObjectUniform(UniformHandle handle, Subresource subresource);
    void SetObjectUniform(UniformHandle handle, ShaderResourceTarget target, uint8_t slot);
    void SetObjectTexture(TextureHandle handle, ShaderResourceTarget target, uint8_t slot);
    void SetObjectSampler(SamplerHandle handle, ShaderResourceTarget target, uint8_t slot);
    void Submit(ProgramHandle handle);

    void Reset()
    {
        for (int i = 0; i < m_num_of_draw_calls; ++i)
            m_draw_data[i].Reset();
        m_num_of_draw_calls = 0;
        m_uniform_cache_offset = 0;
    }

    inline bool HasData() const
    {
        return m_num_of_draw_calls > 0;
    }
    inline bool NeedsFlush() const
    {
        return m_num_of_draw_calls == PPGE_RENDERER_MAX_DRAWCALLS || m_uniform_cache_offset > (1 * 1024 * 1024 - 1024);
    }
    inline size_t GetNumberOfDrawCalls() const
    {
        return m_num_of_draw_calls;
    }

    auto begin()
    {
        return std::begin(m_draw_data);
    }
    auto end()
    {
        return std::begin(m_draw_data) + m_num_of_draw_calls;
    }
    auto begin() const
    {
        return std::cbegin(m_draw_data);
    }
    auto end() const
    {
        return std::begin(m_draw_data) + m_num_of_draw_calls;
    }

  private:
    inline bool ReachedDrawCallLimit() const
    {
        return m_num_of_draw_calls == PPGE_RENDERER_MAX_DRAWCALLS;
    }

    DrawData m_draw_data[PPGE_RENDERER_MAX_DRAWCALLS] = {};
    size_t m_num_of_draw_calls = 0;
    char m_uniform_cache[1 * 1024 * 1024] = {};
    size_t m_uniform_cache_offset = 0;
};
} // namespace PPGE