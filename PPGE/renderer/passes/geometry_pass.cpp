#include "geometry_pass.h"

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
GeometryPass::GeometryPass()
{
    // Create constant buffers
    {
        BufferDesc cb_desc;
        cb_desc.cpu_access_flags = CPUAccessFlags::CPU_ACCESS_WRITE;
        cb_desc.bind_flags = BindFlags::BIND_CONSTANT_BUFFER;
        cb_desc.usage = UsageType::USAGE_DYNAMIC;
        cb_desc.byte_width = sizeof(CbPerDraw);
        RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_per_draw);
        cb_desc.byte_width = sizeof(CbMaterial);
        RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_material);
    }
    // Create Sampler
    {
        SamplerDesc sampler_desc;
        sampler_desc.min_filter = FilterType::FILTER_ANISOTROPIC;
        sampler_desc.mag_filter = FilterType::FILTER_ANISOTROPIC;
        sampler_desc.mip_filter = FilterType::FILTER_ANISOTROPIC;
        sampler_desc.address_u = TextureAddressType::TEXTURE_ADDRESS_WRAP;
        sampler_desc.address_v = TextureAddressType::TEXTURE_ADDRESS_WRAP;
        sampler_desc.address_w = TextureAddressType::TEXTURE_ADDRESS_WRAP;
        sampler_desc.max_anisotropy = 16;
        sampler_desc.comperison_func = ComperisonFuncType::COMPARISON_FUNC_ALWAYS;
        RendererSystem::Get().GetDevice()->CreateSampler(sampler_desc, m_anisotropic_sampler);
    }
    // Create PSOs
    {
        std::shared_ptr<PPGEShader> vs;
        {
            auto &simple_vs = ShaderLibrary::Get().GetShaderCode("common_vs.hlsl");
            ShaderCreateDesc cd;
            cd.desc.shader_type_flags = ShaderTypeFlags::SHADER_TYPE_VERTEX;
            cd.compiler = ShaderCompilerType::SHADER_COMPILER_FXC;
            cd.file_path = "../../PPGE/renderer/shaders/common_vs.hlsl";
            cd.source_code = simple_vs.data.data();
            cd.source_code_size = simple_vs.data.size();
            cd.entry_point_name = "main_VS";
            RendererSystem::Get().GetDevice()->CreateShader(cd, vs);
        }
        std::shared_ptr<PPGEShader> ps;
        {
            auto &simple_ps = ShaderLibrary::Get().GetShaderCode("geometry_pass_ps.hlsl");
            ShaderCreateDesc cd;
            cd.desc.shader_type_flags = ShaderTypeFlags::SHADER_TYPE_PIXEL;
            cd.compiler = ShaderCompilerType::SHADER_COMPILER_FXC;
            cd.file_path = "../../PPGE/renderer/shaders/geometry_pass_ps.hlsl";
            cd.source_code = simple_ps.data.data();
            cd.source_code_size = simple_ps.data.size();
            cd.entry_point_name = "main_PS";
            RendererSystem::Get().GetDevice()->CreateShader(cd, ps);
        }

        GfxPipelineStateCreateDesc ps_cd;

        ps_cd.commited_vs = vs;
        ps_cd.commited_ps = ps;

        auto [elments, element_count] = GetStandardLayout();
        ps_cd.desc.input_layout_desc.elements = elments;
        ps_cd.desc.input_layout_desc.elements_num = element_count;
        ps_cd.desc.rasterizer_state_desc.cull_mode = CullModeType::CULL_MODE_BACK;

        const char *vs_cbs[] = {"cb_renderer", "cb_per_frame", "cb_per_draw"};
        const char *ps_cbs[] = {"cb_renderer", "cb_per_frame", "cb_per_draw", "cb_material"};
        const char *ps_srvs[] = {"g_material_albedo", "g_material_roughness", "g_material_metallic",
                                 "g_material_normal", "g_material_occlusion", "g_material_emission",
                                 "g_material_height", "g_material_alpha_mask"};
        const char *ps_smps[] = {"g_sampler_anisotropic"};

        ShaderResourceRangeCreateDesc range[] = {
            {{ShaderTypeFlags::SHADER_TYPE_VERTEX, ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER, 0u},
             sizeof(vs_cbs) / sizeof(ShaderResourceDesc),
             vs_cbs},
            {{ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER, 0u},
             sizeof(ps_cbs) / sizeof(ShaderResourceDesc),
             ps_cbs},
            {{ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV, 0u},
             sizeof(ps_srvs) / sizeof(ShaderResourceDesc),
             ps_srvs},
            {{ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_SAMPLER, 1u},
             sizeof(ps_smps) / sizeof(ShaderResourceDesc),
             ps_smps},
        };

        ps_cd.sr_create_desc.range = range;
        ps_cd.sr_create_desc.range_num = (sizeof(range) / sizeof(ShaderResourceRangeCreateDesc));

        RendererSystem::Get().GetDevice()->CreatePipelineState(ps_cd, m_PSO);
    }
    // Create Shader Resource Binding
    m_SRB = m_PSO->CreateShaderResourceBinding();
    {
        m_SRB->GetVariableByName("cb_per_draw", ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(m_cb_per_draw);
        m_SRB->GetVariableByName("cb_per_draw", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_cb_per_draw);
        m_SRB->GetVariableByName("cb_material", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_cb_material);
        m_SRB->GetVariableByName("g_sampler_anisotropic", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_anisotropic_sampler);
    }
}

void GeometryPass::Load(RenderGraph &render_graph)
{
    auto cb_renderer = render_graph.GetResource<PPGEBuffer>(CbRendererOptionsName);
    m_SRB->GetVariableByName("cb_renderer", ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(cb_renderer);
    m_SRB->GetVariableByName("cb_renderer", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(std::move(cb_renderer));

    auto cb_per_frame = render_graph.GetResource<PPGEBuffer>(CbPerFrameResourceName);
    m_SRB->GetVariableByName("cb_per_frame", ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(cb_per_frame);
    m_SRB->GetVariableByName("cb_per_frame", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(std::move(cb_per_frame));

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

void GeometryPass::Unload()
{
    m_albedo_buffer_rtv.reset();
    m_normal_buffer_rtv.reset();
    m_position_buffer_rtv.reset();
    m_material_buffer_rtv.reset();
    m_emission_buffer_rtv.reset();
}

void GeometryPass::Execute()
{
    RendererSystem::Get().GetImmediateContext()->SetPipelineStateObject(m_PSO);
    {
        std::shared_ptr<PPGETextureView> RTVs[] = {m_albedo_buffer_rtv, m_normal_buffer_rtv, m_position_buffer_rtv,
                                                   m_material_buffer_rtv, m_emission_buffer_rtv};
        RendererSystem::Get().GetImmediateContext()->SetRenderTargets(5, RTVs, m_depth_buffer_dsv);

        auto text_desc = m_albedo_buffer_rtv->GetTexture()->GetDesc();
        Viewport viewport{.top_left_x = 0.0f,
                          .top_left_y = 0.0f,
                          .width = static_cast<float>(text_desc.width),
                          .height = static_cast<float>(text_desc.height),
                          .min_depth = 0.0f,
                          .max_depth = 1.0f};
        RendererSystem::Get().GetImmediateContext()->SetViewports(1, &viewport);
    }

    const auto &data = GetSceneDataRef();

    for (auto [entity, transform, mesh_filer, mesh_renderer] :
         data.scene.View<const TransformComponent, const MeshFilterComponent, const MeshRendererComponent>().each())
    {
        // Bind vertex and index buffers
        mesh_filer.mesh->Bind();

        // Update material
        mesh_renderer.material->Build(m_SRB, m_cb_material);

        // Update per draw call constant buffer
        CbPerDraw *per_draw_data;
        RendererSystem::Get().GetImmediateContext()->Map(m_cb_per_draw.get(), MapType::MAP_WRITE, MapFlags::MAP_DISCARD,
                                                         reinterpret_cast<void **>(&per_draw_data));
        if (per_draw_data)
        {
            per_draw_data->entity_id = static_cast<uint32_t>(entity);

            auto world = transform.GetWorldMatrix();
#if !defined(PPGE_PLATFORM_APPLE)
            auto world_inv_trans = world.Invert().Transpose();

            per_draw_data->world = world.Transpose();
            per_draw_data->world_inverse_transpose = world_inv_trans.Transpose();
#else
            auto world_inv_trans = simd_transpose(simd_inverse(world));

            per_draw_data->world = simd_transpose(world);
            per_draw_data->world_inverse_transpose = simd_transpose(world_inv_trans);
#endif

            RendererSystem::Get().GetImmediateContext()->Unmap(m_cb_per_draw.get());
        }

        // Bind shader resources
        RendererSystem::Get().GetImmediateContext()->CommitShaderResources(m_SRB);

        // Issue draw call
        if (mesh_filer.mesh->IsIndexBufferBound())
            RendererSystem::Get().GetImmediateContext()->DrawIndexed(mesh_filer.mesh->GetNumIndices());
        else
            RendererSystem::Get().GetImmediateContext()->Draw(0);
    }
}
} // namespace PPGE
