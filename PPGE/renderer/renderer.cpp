#include "renderer.h"

namespace PPGE
{
void Renderer::BeginScene(const SceneData &scene_data)
{
    s_data = scene_data;
}
void Renderer::EndScene()
{
}
RendererAPI Renderer::GetRendererAPI()
{
    return RendererSystem::Get().GetRendererAPI();
}
VertexBufferHandle Renderer::CreateVertexBuffer(const VertexBufferDesc &desc)
{
    return RendererSystem::Get().CreateVertexBuffer(desc);
}
void Renderer::SetVertexBuffer(VertexBufferHandle handle)
{
    RendererSystem::Get().SetVertexBuffer(handle);
}
void Renderer::ReleaseVertexBuffer(VertexBufferHandle &handle)
{
    RendererSystem::Get().ReleaseVertexBuffer(handle);
}
IndexBufferHandle Renderer::CreateIndexBuffer(const IndexBufferDesc &desc)
{
    return RendererSystem::Get().CreateIndexBuffer(desc);
}
void Renderer::SetIndexBuffer(IndexBufferHandle handle)
{
    RendererSystem::Get().SetIndexBuffer(handle);
}
void Renderer::ReleaseIndexBuffer(IndexBufferHandle &handle)
{
    RendererSystem::Get().ReleaseIndexBuffer(handle);
}
TextureHandle Renderer::CreateTexture(const TextureDesc &desc)
{
    return RendererSystem::Get().CreateTexture(desc);
}
void Renderer::SetTexture(TextureHandle handle, Sampler sampler)
{
    RendererSystem::Get().SetTexture(handle, sampler);
}
void Renderer::ReleaseTexture(TextureHandle &handle)
{
    RendererSystem::Get().ReleaseTexture(handle);
}
UniformHandle Renderer::CreateUniform(const UniformDesc &desc)
{
    return RendererSystem::Get().CreateUniform(desc);
}
void Renderer::SetUniform(UniformHandle handle, void *data)
{
    RendererSystem::Get().SetUniform(handle, data);
}
void Renderer::SetRenderStates(const RenderStates &states)
{
    RendererSystem::Get().SetRenderStates(states);
}
void Renderer::Submit(ProgramHandle handle)
{
    RendererSystem::Get().Submit(handle);
}
} // namespace PPGE