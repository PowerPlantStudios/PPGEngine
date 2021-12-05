#include "renderer.h"

namespace PPGE
{

SceneData Renderer::s_scene_data{};
HandleAllocator<VertexBufferHandle> Renderer::s_vb_handle_alloc;
HandleAllocator<IndexBufferHandle> Renderer::s_ib_handle_alloc;
HandleAllocator<TextureHandle> Renderer::s_texture_handle_alloc;
HandleAllocator<ProgramHandle> Renderer::s_program_handle_alloc;
HandleAllocator<UniformHandle> Renderer::s_uniform_handle_alloc;

void Renderer::BeginScene(const SceneData &scene_data)
{
    s_scene_data = scene_data;
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
    VertexBufferHandle handle = s_vb_handle_alloc.GetNext();
    PPGE_ASSERT(handle.IsValid(), "No handle is available to create vertex buffer.");
    if (!RendererSystem::Get().CreateVertexBuffer(desc, handle))
    {
        PPGE_ASSERT(false, "Creating renderer resource has failed for vertex buffer.");
        s_vb_handle_alloc.Restore(handle);
        handle = VertexBufferHandle{};
    }
    return handle;
}
void Renderer::SetVertexBuffer(VertexBufferHandle handle)
{
    PPGE_ASSERT(handle.IsValid(), "Invalid handle to vertex buffer is passed for binding resource.");
    PPGE_ASSERT(RendererSystem::Get().SetVertexBuffer(handle),
                "Binding renderer resource has failed for vertex buffer.");
}
void Renderer::ReleaseVertexBuffer(VertexBufferHandle &handle)
{
    PPGE_ASSERT(handle.IsValid(), "Invalid handle to vertex buffer is passed for releasing resource.");
    PPGE_ASSERT(RendererSystem::Get().ReleaseVertexBuffer(handle),
                "Releasing renderer resource has failed for vertex buffer.");
    s_vb_handle_alloc.Restore(handle);
}
IndexBufferHandle Renderer::CreateIndexBuffer(const IndexBufferDesc &desc)
{
    IndexBufferHandle handle = s_ib_handle_alloc.GetNext();
    PPGE_ASSERT(handle.IsValid(), "No handle is available to create index buffer.");
    if (!RendererSystem::Get().CreateIndexBuffer(desc, handle))
    {
        PPGE_ASSERT(false, "Creating renderer resource has failed for index buffer.");
        s_ib_handle_alloc.Restore(handle);
        handle = IndexBufferHandle{};
    }
    return handle;
}
void Renderer::SetIndexBuffer(IndexBufferHandle handle)
{
    PPGE_ASSERT(handle.IsValid(), "Invalid handle to index buffer is passed for binding resource.");
    PPGE_ASSERT(RendererSystem::Get().SetIndexBuffer(handle), "Binding renderer resource has failed for index buffer.");
}
void Renderer::ReleaseIndexBuffer(IndexBufferHandle &handle)
{
    PPGE_ASSERT(handle.IsValid(), "Invalid handle to index buffer is passed for releasing resource.");
    PPGE_ASSERT(RendererSystem::Get().ReleaseIndexBuffer(handle),
                "Releasing renderer resource has failed for index buffer.");
    s_ib_handle_alloc.Restore(handle);
}
TextureHandle Renderer::CreateTexture(const TextureDesc &desc)
{
    TextureHandle handle = s_texture_handle_alloc.GetNext();
    PPGE_ASSERT(handle.IsValid(), "No handle is available to create texture resource.");
    if (!RendererSystem::Get().CreateTexture(desc, handle))
    {
        PPGE_ASSERT(false, "Creating renderer resource has failed for texture.");
        s_texture_handle_alloc.Restore(handle);
        handle = TextureHandle{};
    }
    return handle;
}
void Renderer::SetTexture(TextureHandle handle, Sampler sampler)
{
    PPGE_ASSERT(handle.IsValid(), "Invalid handle to texture is passed for binding resource.");
    PPGE_ASSERT(RendererSystem::Get().SetTexture(handle, sampler),
                "Binding renderer resource has failed for texture.");
}
void Renderer::ReleaseTexture(TextureHandle &handle)
{
    PPGE_ASSERT(handle.IsValid(), "Invalid handle to texture is passed for releasing resource.");
    PPGE_ASSERT(RendererSystem::Get().ReleaseTexture(handle),
                "Releasing renderer resource has failed for texture.");
    s_texture_handle_alloc.Restore(handle);
}
UniformHandle Renderer::CreateUniform(const UniformDesc &desc)
{
    UniformHandle handle = s_uniform_handle_alloc.GetNext();
    PPGE_ASSERT(handle.IsValid(), "No handle is available to create uniform resource.");
    if (!RendererSystem::Get().CreateUniform(desc, handle))
    {
        PPGE_ASSERT(false, "Creating renderer resource has failed for uniform.");
        s_uniform_handle_alloc.Restore(handle);
        handle = UniformHandle{};
    }
    return handle;
}
void Renderer::SetUniform(UniformHandle handle, void *data)
{
    PPGE_ASSERT(handle.IsValid(), "Invalid handle to uniform is passed for binding resource.");
    PPGE_ASSERT(RendererSystem::Get().SetUniform(handle, data), "Binding renderer resource has failed for uniform.");
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