#include "forward_render_pass.h"

#include "ecs/components/mesh_component.h"
#include "ecs/components/transform_components.h"
#include "ecs/scene.h"
#include "renderer/render_graph.h"
#include "renderer/renderer_constants.h"
#include "renderer/renderer_types.h"
#include "renderer/shader_library.h"
#include "system/renderer_system.h"

namespace PPGE
{
struct cbVS
{
    Math::Matrix model;
    Math::Matrix modelInvTran;
};

struct cbPS
{
    Math::Color albedo;
    Math::Color specular;
};

ForwardRenderPass::ForwardRenderPass()
{
    // Create constant buffers
    {
        BufferDesc cb_desc;
        cb_desc.byte_width = sizeof(cbVS);
        cb_desc.bind_flags = BindFlags::BIND_CONSTANT_BUFFER;
        cb_desc.usage = UsageType::USAGE_DYNAMIC;
        cb_desc.cpu_access_flags = CPUAccessFlags::CPU_ACCESS_WRITE;
        RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_vs);
        cb_desc.byte_width = sizeof(cbPS);
        RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_ps);
    }

    // Create Sampler
    {
        SamplerDesc sampler_desc;
        RendererSystem::Get().GetDevice()->CreateSampler(sampler_desc, m_sampler_state);
    }

    // Create Pipeline State Object
    {
        std::shared_ptr<PPGE::PPGEShader> vs;
        {
            auto &simple_vs = ShaderLibrary::Get().GetShaderCode("simple_vs.hlsl");
            PPGE::ShaderCreateDesc cd;
            cd.desc.shader_type_flags = PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX;
            cd.compiler = PPGE::ShaderCompilerType::SHADER_COMPILER_FXC;
            cd.source_code = simple_vs.data.data();
            cd.source_code_size = simple_vs.data.size();
            PPGE::RendererSystem::Get().GetDevice()->CreateShader(cd, vs);
        }

        std::shared_ptr<PPGE::PPGEShader> ps;
        {
            auto &simple_ps = ShaderLibrary::Get().GetShaderCode("simple_ps.hlsl");
            PPGE::ShaderCreateDesc cd;
            cd.desc.shader_type_flags = PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL;
            cd.compiler = PPGE::ShaderCompilerType::SHADER_COMPILER_FXC;
            cd.source_code = simple_ps.data.data();
            cd.source_code_size = simple_ps.data.size();
            PPGE::RendererSystem::Get().GetDevice()->CreateShader(cd, ps);
        }

        PPGE::GfxPipelineStateCreateDesc ps_cd;
        ps_cd.desc.input_layout_desc.elements = GetLayout("FullLayout");
        ps_cd.desc.input_layout_desc.elements_num = sizeof(ps_cd.desc.input_layout_desc.elements);
        ps_cd.desc.rasterizer_state_desc.cull_mode = PPGE::CullModeType::CULL_MODE_NONE;
        ps_cd.commited_vs = vs;
        ps_cd.commited_ps = ps;

        PPGE::ShaderResourceCreateDesc SRVs[] = {
            {"cb_per_frame",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX, PPGE::ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_per_object",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX, PPGE::ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_per_object",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"g_diffuse",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_sampler",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_SAMPLER}}};
        ps_cd.srv = SRVs;
        ps_cd.srv_num = (sizeof(SRVs) / sizeof(PPGE::ShaderResourceCreateDesc));

        PPGE::RendererSystem::Get().GetDevice()->CreatePipelineState(ps_cd, m_PSO);
    }

    // Create Shader Resource Binding
    m_SRB = m_PSO->CreateShaderResourceBinding();
    m_SRB->GetVariableByName("cb_per_object", PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(m_cb_vs);
    m_SRB->GetVariableByName("cb_per_object", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_cb_ps);
    m_SRB->GetVariableByName("g_sampler", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_sampler_state);
}

void ForwardRenderPass::Load(RenderGraph &render_graph)
{
    auto cb_per_frame = render_graph.GetResource<PPGEBuffer>(CbCameraDataResourceName);
    m_SRB->GetVariableByName("cb_per_frame", PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(std::move(cb_per_frame));

    auto shadow_map_buffer = render_graph.GetResource<PPGETexture>(RenderPassResourceDescs::Shadow_Map_Resource);
    {
        TextureViewDesc desc;
        desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE;
        desc.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D;
        desc.format = TextureFormatType::TEXTURE_FORMAT_R24_UNORM_X8_TYPELESS;
        desc.most_detailed_mip = 0;
        desc.mip_levels_num = 1;
        m_shadow_buffer_srv = shadow_map_buffer->CreateView(desc);
    }

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

void ForwardRenderPass::Unload()
{
    m_shadow_buffer_srv.reset();
    m_depth_buffer_dsv.reset();
    m_color_buffer_rtv.reset();
}

void ForwardRenderPass::Execute()
{
    const auto &data = GetSceneDataRef();

    RendererSystem::Get().GetImmediateContext()->SetPipelineStateObject(m_PSO);

    for (auto [entity, transform, mesh_filer, mesh_renderer] :
         data.scene.View<const TransformComponent, const MeshFilterComponent, const MeshRendererComponent>()
             .each())
    {
        // Set per object shader resources
        m_SRB->GetVariableByName("g_diffuse", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(mesh_renderer.albedo_map);
        PPGE::RendererSystem::Get().GetImmediateContext()->CommitShaderResources(m_SRB);

        // Bind vertex and index buffers
        {
            std::shared_ptr<PPGE::PPGEBuffer> vbs[] = {mesh_filer.vertex_buffer};
            uint64_t offsets[] = {0};
            PPGE::RendererSystem::Get().GetImmediateContext()->SetVertexBuffers(1, vbs, offsets);
            PPGE::RendererSystem::Get().GetImmediateContext()->SetIndexBuffer(mesh_filer.index_buffer);
        }

        // Update per object VS constant buffer
        {
            cbVS *map_data;
            PPGE::RendererSystem::Get().GetImmediateContext()->Map(m_cb_vs.get(), PPGE::MapType::MAP_WRITE,
                                                                   PPGE::MapFlags::MAP_DISCARD,
                                                                   reinterpret_cast<void **>(&map_data));
            auto model = transform.GetModelMatrix();
            auto inv_mode = model.Invert();
            *map_data = cbVS{.model = model, .modelInvTran = inv_mode};
            PPGE::RendererSystem::Get().GetImmediateContext()->Unmap(m_cb_vs.get());
        }

        // Draw indexed
        PPGE::RendererSystem::Get().GetImmediateContext()->DrawIndexed(mesh_filer.num_indices);
    }
}
} // namespace PPGE