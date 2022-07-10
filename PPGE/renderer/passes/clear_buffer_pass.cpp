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
    auto albedo_buffer = render_graph.GetResource<PPGETexture>(RenderPassResourceDescs::Albedo_Buffer_Resource);
    {
        TextureViewDesc desc;
        desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_RENDER_TARGET;
        desc.resource_dimension = albedo_buffer->GetDesc().resource_dimension;
        desc.format = albedo_buffer->GetDesc().format_type;
        desc.most_detailed_mip = 0;
        desc.mip_levels_num = 1;
        desc.array_slices_num = albedo_buffer->GetDesc().array_size;
        desc.first_array_slice = 0;
        m_albedo_buffer_rtv = albedo_buffer->CreateView(desc);
    }

    auto normal_buffer = render_graph.GetResource<PPGETexture>(RenderPassResourceDescs::Normal_Buffer_Resource);
    {
        TextureViewDesc desc;
        desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_RENDER_TARGET;
        desc.resource_dimension = normal_buffer->GetDesc().resource_dimension;
        desc.format = normal_buffer->GetDesc().format_type;
        desc.most_detailed_mip = 0;
        desc.mip_levels_num = 1;
        desc.array_slices_num = normal_buffer->GetDesc().array_size;
        desc.first_array_slice = 0;
        m_normal_buffer_rtv = normal_buffer->CreateView(desc);
    }

    auto position_buffer = render_graph.GetResource<PPGETexture>(RenderPassResourceDescs::Position_Buffer_Resource);
    {
        TextureViewDesc desc;
        desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_RENDER_TARGET;
        desc.resource_dimension = position_buffer->GetDesc().resource_dimension;
        desc.format = position_buffer->GetDesc().format_type;
        desc.most_detailed_mip = 0;
        desc.mip_levels_num = 1;
        desc.array_slices_num = position_buffer->GetDesc().array_size;
        desc.first_array_slice = 0;
        m_position_buffer_rtv = position_buffer->CreateView(desc);
    }

    auto material_buffer = render_graph.GetResource<PPGETexture>(RenderPassResourceDescs::Material_Buffer_Resource);
    {
        TextureViewDesc desc;
        desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_RENDER_TARGET;
        desc.resource_dimension = material_buffer->GetDesc().resource_dimension;
        desc.format = material_buffer->GetDesc().format_type;
        desc.most_detailed_mip = 0;
        desc.mip_levels_num = 1;
        desc.array_slices_num = material_buffer->GetDesc().array_size;
        desc.first_array_slice = 0;
        m_material_buffer_rtv = material_buffer->CreateView(desc);
    }

    auto emission_buffer = render_graph.GetResource<PPGETexture>(RenderPassResourceDescs::Emission_Buffer_Resource);
    {
        TextureViewDesc desc;
        desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_RENDER_TARGET;
        desc.resource_dimension = emission_buffer->GetDesc().resource_dimension;
        desc.format = emission_buffer->GetDesc().format_type;
        desc.most_detailed_mip = 0;
        desc.mip_levels_num = 1;
        desc.array_slices_num = emission_buffer->GetDesc().array_size;
        desc.first_array_slice = 0;
        m_emission_buffer_rtv = emission_buffer->CreateView(desc);
    }

    auto color_buffer = render_graph.GetResource<PPGETexture>(RenderPassResourceDescs::Present_Buffer_Resource);
    {
        TextureViewDesc desc;
        desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_RENDER_TARGET;
        desc.resource_dimension = color_buffer->GetDesc().resource_dimension;
        desc.format = color_buffer->GetDesc().format_type;
        desc.most_detailed_mip = 0;
        desc.mip_levels_num = 1;
        desc.array_slices_num = color_buffer->GetDesc().array_size;
        desc.first_array_slice = 0;
        m_present_buffer_rtv = color_buffer->CreateView(desc);
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
    RendererSystem::Get().GetImmediateContext()->ClearRenderTarget(m_albedo_buffer_rtv, 0.0f, 0.0f, 0.0f, 1.0f);
    RendererSystem::Get().GetImmediateContext()->ClearRenderTarget(m_normal_buffer_rtv, 0.0f, 0.0f, 0.0f, 1.0f);
    RendererSystem::Get().GetImmediateContext()->ClearRenderTarget(m_position_buffer_rtv, 0.0f, 0.0f, 0.0f, 1.0f);
    RendererSystem::Get().GetImmediateContext()->ClearRenderTarget(m_material_buffer_rtv, 0.0f, 0.0f, 0.0f, 1.0f);
    RendererSystem::Get().GetImmediateContext()->ClearRenderTarget(m_emission_buffer_rtv, 0.0f, 0.0f, 0.0f, 1.0f);
    RendererSystem::Get().GetImmediateContext()->ClearRenderTarget(m_present_buffer_rtv, 0.0f, 0.0f, 0.0f, 1.0f);
    RendererSystem::Get().GetImmediateContext()->ClearDepthStencil(m_depth_buffer_dsv, 1.0f, 0);
}
} // namespace PPGE
