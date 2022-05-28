#include "clear_buffer_pass.h"

#include "renderer/render_graph.h"
#include "system/renderer_system.h"

namespace PPGE
{
ClearBufferPass::ClearBufferPass()
{
}

void ClearBufferPass::Load(RenderGraph &render_graph)
{
    auto color_buffer = render_graph.GetResource<PPGETexture>(RenderPassResourceDescs::Color_Buffer_Resource);
    {
        TextureViewDesc desc;
        desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_RENDER_TARGET;
        desc.resource_dimension = color_buffer->GetDesc().resource_dimension;
        desc.format = color_buffer->GetDesc().format_type;
        desc.most_detailed_mip = 0;
        desc.mip_levels_num = 1;
        desc.array_slices_num = color_buffer->GetDesc().array_size;
        desc.first_array_slice = 0;
        m_color_buffer_rtv = color_buffer->CreateView(desc);
    }

    auto depth_buffer = render_graph.GetResource<PPGETexture>(RenderPassResourceDescs::Depth_Buffer_Resource);
    {
        TextureViewDesc desc;
        desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_DEPTH_STENCIL;
        desc.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D;
        desc.format = depth_buffer->GetDesc().format_type;
        desc.most_detailed_mip = 0;
        desc.mip_levels_num = 1;
        m_depth_buffer_dsv = depth_buffer->CreateView(desc);
    }
}

void ClearBufferPass::Unload()
{
}

void ClearBufferPass::Execute()
{
    RendererSystem::Get().GetImmediateContext()->ClearRenderTarget(m_color_buffer_rtv, 0.6, 0.8, 0.9, 1.0f);
    RendererSystem::Get().GetImmediateContext()->ClearDepthStencil(m_depth_buffer_dsv, 1.0f, 0);
}
} // namespace PPGE
