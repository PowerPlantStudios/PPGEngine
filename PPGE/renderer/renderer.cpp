#include "renderer.h"

namespace PPGE
{
Frame Renderer::s_frame_data{};
Renderer::VertexLayoutCache Renderer::s_vertex_layout_cash;
Renderer::VertexBufferLayoutTable Renderer::s_vertex_buffer_layout_table;
HandleAllocator<VertexBufferHandle> Renderer::s_vb_handle_alloc;
HandleAllocator<VertexLayoutHandle> Renderer::s_layout_handle_alloc;
HandleAllocator<IndexBufferHandle> Renderer::s_ib_handle_alloc;
HandleAllocator<TextureHandle> Renderer::s_texture_handle_alloc;
HandleAllocator<ShaderHandle> Renderer::s_shader_handle_alloc;
HandleAllocator<ProgramHandle> Renderer::s_program_handle_alloc;
HandleAllocator<UniformHandle> Renderer::s_uniform_handle_alloc;

void Renderer::BeginScene(const SceneData &scene_data)
{
    struct VSPerFrameData
    {
        Math::Matrix g_view;
        Math::Matrix g_proj;
        Math::Matrix g_viewproj;
    } vs_per_frame_data{scene_data.m_view, scene_data.m_projection, scene_data.m_projection * scene_data.m_view};

    PredefinedUniform pred_un;
    pred_un.handle = UniformHandle{static_cast<uint8_t>(PredefinedUniform::Transform0)};
    pred_un.subresource.m_pData = &vs_per_frame_data;
    pred_un.subresource.m_size = sizeof(VSPerFrameData);
    pred_un.target = UniformDesc::Target::VS;
    pred_un.slot = 0;
    bool result = RendererSystem::Get().SetPredefinedUniform(pred_un);
    PPGE_ASSERT(result, "Setting predefined uniform has failed.");

    s_frame_data.Reset();
}
void Renderer::EndScene()
{
    Flush();
}
RendererAPI Renderer::GetRendererAPI()
{
    return RendererSystem::Get().GetRendererAPI();
}
VertexBufferHandle Renderer::CreateVertexBuffer(const VertexBufferDesc &desc)
{
    VertexBufferHandle vb_handle = s_vb_handle_alloc.GetNext();
    PPGE_ASSERT(vb_handle.IsValid(), "No handle is available to create vertex buffer.");
    VertexLayoutHandle ly_handle = FindOrCreateVertexLayout(desc.m_layout);
    PPGE_ASSERT(ly_handle.IsValid(), "Handle to vertex layout is invalid.");

    if (!RendererSystem::Get().CreateVertexBuffer(desc, vb_handle))
    {
        PPGE_ASSERT(false, "Creating renderer resource has failed for vertex buffer.");
        s_vb_handle_alloc.Restore(vb_handle);
        s_layout_handle_alloc.Restore(ly_handle);
        return VertexBufferHandle{};
    }
    s_vertex_buffer_layout_table[vb_handle.idx] = ly_handle;

    return vb_handle;
}
void Renderer::SetVertexBuffer(VertexBufferHandle handle)
{
    PPGE_ASSERT(handle.IsValid(), "Invalid handle to vertex buffer is passed for binding resource.");
    s_frame_data.SetVertexBufferHandle(handle);
    auto search = s_vertex_buffer_layout_table.find(handle.idx);
    PPGE_ASSERT(search != s_vertex_buffer_layout_table.end(),
                "Vertex layout associated with vertex buffer cannot be found.");
    s_frame_data.SetVertexLayoutHandle(search->second);
}
void Renderer::ReleaseVertexBuffer(VertexBufferHandle &handle)
{
    PPGE_ASSERT(handle.IsValid(), "Invalid handle to vertex buffer is passed for releasing resource.");
    bool result = RendererSystem::Get().ReleaseVertexBuffer(handle);
    PPGE_ASSERT(result, "Releasing renderer resource has failed for vertex buffer.");
    s_vb_handle_alloc.Restore(handle);

    auto search = s_vertex_buffer_layout_table.find(handle.idx);
    PPGE_ASSERT(search != s_vertex_buffer_layout_table.end(),
                "Vertex layout associated with vertex buffer cannot be found.");
    VertexLayoutHandle ly_handle = search->second;
    s_vertex_buffer_layout_table.erase(search);

    s_layout_handle_alloc.Restore(ly_handle);
    if (!s_layout_handle_alloc.HandleIsUsed(ly_handle))
    {
        bool result = RendererSystem::Get().ReleaseVertexLayout(ly_handle);
        PPGE_ASSERT(result, "Releasing renderer resource has failed for vertex layout.");
        for (auto it = s_vertex_layout_cash.begin(); it != s_vertex_layout_cash.end();)
        {
            if (it->second == ly_handle)
            {
                s_vertex_layout_cash.erase(it++);
            }
            else
            {
                it++;
            }
        }
    }
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
    s_frame_data.SetIndexBufferHandle(handle);
}
void Renderer::ReleaseIndexBuffer(IndexBufferHandle &handle)
{
    PPGE_ASSERT(handle.IsValid(), "Invalid handle to index buffer is passed for releasing resource.");
    bool result = RendererSystem::Get().ReleaseIndexBuffer(handle);
    PPGE_ASSERT(result, "Releasing renderer resource has failed for index buffer.");
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
void Renderer::SetTexture(TextureHandle handle, TextureDesc::Sampler sampler)
{
    PPGE_ASSERT(handle.IsValid(), "Invalid handle to texture is passed for binding resource.");
}
void Renderer::ReleaseTexture(TextureHandle &handle)
{
    PPGE_ASSERT(handle.IsValid(), "Invalid handle to texture is passed for releasing resource.");
    bool result = RendererSystem::Get().ReleaseTexture(handle);
    PPGE_ASSERT(result, "Releasing renderer resource has failed for texture.");
    s_texture_handle_alloc.Restore(handle);
}
ShaderHandle Renderer::CreateShader(const ShaderDesc &desc)
{
    ShaderHandle handle = s_shader_handle_alloc.GetNext();
    PPGE_ASSERT(handle.IsValid(), "No handle is available to create shader.");
    if (!RendererSystem::Get().CreateShader(desc, handle))
    {
        PPGE_ASSERT(false, "Creating renderer resource has failed for shader.");
        s_shader_handle_alloc.Restore(handle);
        handle = ShaderHandle{};
    }
    return handle;
}
ProgramHandle Renderer::CreateProgram(const ProgramDesc &desc)
{
    ProgramHandle handle = s_program_handle_alloc.GetNext();
    PPGE_ASSERT(handle.IsValid(), "No handle is available to create program.");
    if (!RendererSystem::Get().CreateProgram(desc, handle))
    {
        PPGE_ASSERT(false, "Creating renderer resource has failed for program.");
        s_program_handle_alloc.Restore(handle);
        handle = ProgramHandle{};
    }
    return handle;
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
void Renderer::UpdateUniform(UniformHandle handle, const Subresource &subresource)
{
    PPGE_ASSERT(handle.IsValid(), "Invalid handle to uniform is passed for binding resource.");
    PPGE_ASSERT(subresource.m_size <= 1024, "Single per object uniform uniform size limit has exceeded.");
    s_frame_data.UpdateObjectUniform(handle, subresource);
}
void Renderer::SetUniform(UniformHandle handle, UniformDesc::Target target, uint8_t slot)
{
    PPGE_ASSERT(handle.IsValid(), "Invalid handle to uniform is passed for binding resource.");
    s_frame_data.SetObjectUniform(handle, target, slot);
}
void Renderer::ReleaseUniform(UniformHandle handle)
{
    PPGE_ASSERT(handle.IsValid(), "Invalid handle to uniform is passed for binding resource.");
    bool result = RendererSystem::Get().ReleaseUniform(handle);
    PPGE_ASSERT(result, "Releasing renderer resource has failed for uniform.");
    s_uniform_handle_alloc.Restore(handle);
}
void Renderer::SetRenderStates(const RenderStates &states)
{
    RendererSystem::Get().SetRenderStates(states);
}
void Renderer::Submit(ProgramHandle handle)
{
    PPGE_ASSERT(handle.IsValid(), "Invalid handle to program is passed for submitting draw call.");
    s_frame_data.Submit(handle);
    if (s_frame_data.NeedsFlush())
        Flush();
}
void Renderer::Flush()
{
    if (s_frame_data.HasData())
        RendererSystem::Get().Submit(s_frame_data);
    // When multi-threaded renderer is implemented you can put barier here
    s_frame_data.Reset();
}
VertexLayoutHandle Renderer::FindOrCreateVertexLayout(const VertexLayout &layout)
{
    VertexLayoutHandle handle;
    auto search = s_vertex_layout_cash.find(layout.GetHashCode());
    if (search != s_vertex_layout_cash.end())
    {
        handle = search->second;
        s_layout_handle_alloc.ShareResource(handle);
        return handle;
    }

    handle = s_layout_handle_alloc.GetNext();
    PPGE_ASSERT(handle.IsValid(), "No handle is available to create vertex layout.");
    if (!RendererSystem::Get().CreateVertexLayout(layout, handle))
    {
        PPGE_ASSERT(false, "Creating renderer resource has failed for vertex layout.");
        s_layout_handle_alloc.Restore(handle);
        return VertexLayoutHandle{};
    }

    s_vertex_layout_cash[layout.GetHashCode()] = handle;

    return handle;
}
} // namespace PPGE