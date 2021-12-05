#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/buffer.h"
#include "renderer/render_states.h"
#include "renderer/renderer_handles.h"
#include "renderer/shader.h"
#include "renderer/texture.h"
#include "renderer/uniform.h"
#include "system/renderer_system.h"

namespace PPGE
{
struct SceneData
{
};

class Renderer
{
  public:
    static void BeginScene(const SceneData &scene_data);
    static void EndScene();

    static RendererAPI GetRendererAPI();

    static VertexBufferHandle CreateVertexBuffer(const VertexBufferDesc &desc);
    static void SetVertexBuffer(VertexBufferHandle handle);
    static void ReleaseVertexBuffer(VertexBufferHandle &handle);

    static IndexBufferHandle CreateIndexBuffer(const IndexBufferDesc &desc);
    static void SetIndexBuffer(IndexBufferHandle handle);
    static void ReleaseIndexBuffer(IndexBufferHandle &handle);

    static TextureHandle CreateTexture(const TextureDesc &desc);
    static void SetTexture(TextureHandle handle, Sampler sampler);
    static void ReleaseTexture(TextureHandle &handle);

    static UniformHandle CreateUniform(const UniformDesc &desc);
    static void SetUniform(UniformHandle handle, void *data);

    static void SetRenderStates(const RenderStates &states);

    static void Submit(ProgramHandle handle);

  private:
    static SceneData s_scene_data;
    static HandleAllocator<VertexBufferHandle> s_vb_handle_alloc;
    static HandleAllocator<IndexBufferHandle> s_ib_handle_alloc;
    static HandleAllocator<TextureHandle> s_texture_handle_alloc;
    static HandleAllocator<ProgramHandle> s_program_handle_alloc;
    static HandleAllocator<UniformHandle> s_uniform_handle_alloc;
};
} // namespace PPGE