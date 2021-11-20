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
    static SceneData s_data;
};
} // namespace PPGE