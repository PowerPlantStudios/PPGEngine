#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/descriptors.h"
#include "renderer/frame.h"
#include "renderer/render_states.h"
#include "renderer/renderer_handles.h"
#include "system/renderer_system.h"

namespace PPGE
{
struct SceneData
{
    Math::Matrix m_projection = Math::Matrix::Identity;
    Math::Matrix m_view = Math::Matrix::Identity;
};

class Renderer
{
  public:
    static void BeginScene(const SceneData &scene_data);
    static void EndScene();

    static RendererAPI GetRendererAPI();

    static VertexBufferHandle CreateVertexBuffer(const VertexBufferDesc &desc);
    static void SetVertexBuffer(VertexBufferHandle handle);
    static void ReleaseVertexBuffer(VertexBufferHandle handle);

    static IndexBufferHandle CreateIndexBuffer(const IndexBufferDesc &desc);
    static void SetIndexBuffer(IndexBufferHandle handle);
    static void ReleaseIndexBuffer(IndexBufferHandle handle);

    static TextureHandle CreateTexture(const TextureDesc &desc);
    static TextureHandle CreateTexture(const Texture2DDesc &desc);
    static TextureHandle CreateTexture(const Texture3DDesc &desc);
    static TextureHandle CreateTexture(const TextureResurceDesc &desc);
    static void SetTexture(TextureHandle handle, ShaderResourceTarget target, uint8_t slot);
    static void ReleaseTexture(TextureHandle handle);

    static SamplerHandle CreateSampler(const SamplerDesc &desc);
    static void SetSampler(SamplerHandle handle, ShaderResourceTarget target, uint8_t slot);
    static void ReleaseSampler(SamplerHandle handle);

    static ShaderHandle CreateShader(const ShaderDesc &desc);
    static ProgramHandle CreateProgram(const ProgramDesc &desc);

    static UniformHandle CreateUniform(const UniformDesc &desc);
    static void UpdateUniform(UniformHandle handle, const Subresource &subresource);
    static void SetUniform(UniformHandle handle, ShaderResourceTarget target, uint8_t slot);
    static void ReleaseUniform(UniformHandle handle);

    static void SetRenderStates(const RenderStates &states);

    static void Submit(ProgramHandle handle);

  private:
    static void Flush();
    static VertexLayoutHandle FindOrCreateVertexLayout(const VertexLayout &layout);

  private:
    using VertexLayoutCache = std::unordered_map<uint64_t, VertexLayoutHandle>;
    static VertexLayoutCache s_vertex_layout_cash;
    using VertexBufferLayoutTable = std::unordered_map<uint64_t, VertexLayoutHandle>;
    static VertexBufferLayoutTable s_vertex_buffer_layout_table;

    static Frame s_frame_data;

    static HandleAllocator<VertexBufferHandle> s_vb_handle_alloc;
    static HandleAllocator<VertexLayoutHandle> s_layout_handle_alloc;
    static HandleAllocator<IndexBufferHandle> s_ib_handle_alloc;
    static HandleAllocator<TextureHandle> s_texture_handle_alloc;
    static HandleAllocator<SamplerHandle> s_sampler_handle_alloc;
    static HandleAllocator<ShaderHandle> s_shader_handle_alloc;
    static HandleAllocator<ProgramHandle> s_program_handle_alloc;
    static HandleAllocator<UniformHandle> s_uniform_handle_alloc;
};
} // namespace PPGE