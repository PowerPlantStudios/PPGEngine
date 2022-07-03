#include "forward_render_pass.h"

#include "ecs/components/light_component.h"
#include "ecs/components/mesh_component.h"
#include "ecs/components/transform_components.h"
#include "ecs/scene.h"
#include "renderer/camera_controller.h"
#include "renderer/light.h"
#include "renderer/render_graph.h"
#include "renderer/renderer_constants.h"
#include "renderer/renderer_types.h"
#include "renderer/shader_library.h"
#include "system/renderer_system.h"

namespace PPGE
{
ForwardRenderPass::ForwardRenderPass()
{
    // Create constant buffers
    {
        BufferDesc cb_desc;
        cb_desc.byte_width = sizeof(CbPerDraw);
        cb_desc.bind_flags = BindFlags::BIND_CONSTANT_BUFFER;
        cb_desc.usage = UsageType::USAGE_DYNAMIC;
        cb_desc.cpu_access_flags = CPUAccessFlags::CPU_ACCESS_WRITE;
        RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_per_draw);
        cb_desc.byte_width = sizeof(CbMaterial);
        RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_material);
        cb_desc.byte_width = sizeof(CbLight);
        RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_light);
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
    {
        SamplerDesc sampler_desc;
        sampler_desc.min_filter = FilterType::FILTER_COMPARISON_LINEAR;
        sampler_desc.mag_filter = FilterType::FILTER_COMPARISON_LINEAR;
        sampler_desc.mip_filter = FilterType::FILTER_COMPARISON_POINT;
        sampler_desc.comperison_func = ComperisonFuncType::COMPARISON_FUNC_LESS;
        RendererSystem::Get().GetDevice()->CreateSampler(sampler_desc, m_comparison_sampler);
    }

    // Create missing texture
    {
        TextureCreateDesc tex_cd;
        tex_cd.desc.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D;
        tex_cd.desc.width = 1;
        tex_cd.desc.height = 1;
        tex_cd.desc.format_type = TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_UNORM;
        tex_cd.desc.bind_flags = BindFlags::BIND_SHADER_RESOURCE;

        uint32_t magenta[] = {0xffff00ff};
        TextureData tex_data;
        tex_data.data_ptr = magenta;
        tex_data.data_size = sizeof(magenta);
        tex_data.pitch = 1;

        tex_cd.subresource = &tex_data;
        tex_cd.subresource_num = 1;

        std::shared_ptr<PPGETexture> tex;
        RendererSystem::Get().GetDevice()->CreateTexture(tex_cd, tex);

        TextureViewDesc view_desc;
        view_desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE;
        view_desc.resource_dimension = tex_cd.desc.resource_dimension;
        view_desc.format = tex_cd.desc.format_type;
        view_desc.most_detailed_mip = 0;
        view_desc.mip_levels_num = 1;
        m_unknown_texture = tex->CreateView(view_desc);
    }

    // Create PSOs
    {
        // Create ambient pass PSO
        std::shared_ptr<PPGEShader> vs;
        {
            auto &simple_vs = ShaderLibrary::Get().GetShaderCode("forward_pass_vs.hlsl");
            ShaderCreateDesc cd;
            cd.desc.shader_type_flags = ShaderTypeFlags::SHADER_TYPE_VERTEX;
            cd.compiler = ShaderCompilerType::SHADER_COMPILER_FXC;
            cd.file_path = "../../PPGE/renderer/shaders/forward_pass_vs.hlsl";
            cd.source_code = simple_vs.data.data();
            cd.source_code_size = simple_vs.data.size();
            cd.entry_point_name = "main_VS";
            RendererSystem::Get().GetDevice()->CreateShader(cd, vs);
        }

        std::shared_ptr<PPGEShader> ambient_ps;
        {
            auto &simple_ps = ShaderLibrary::Get().GetShaderCode("forward_pass_ps.hlsl");
            ShaderCreateDesc cd;
            cd.desc.shader_type_flags = ShaderTypeFlags::SHADER_TYPE_PIXEL;
            cd.compiler = ShaderCompilerType::SHADER_COMPILER_FXC;
            cd.file_path = "../../PPGE/renderer/shaders/forward_pass_ps.hlsl";
            cd.source_code = simple_ps.data.data();
            cd.source_code_size = simple_ps.data.size();
            cd.entry_point_name = "ambient_pass_PS";
            RendererSystem::Get().GetDevice()->CreateShader(cd, ambient_ps);
        }

        GfxPipelineStateCreateDesc ps_cd;

        ps_cd.commited_vs = vs;
        ps_cd.commited_ps = ambient_ps;

        auto [elments, element_count] = GetStandardLayout();
        ps_cd.desc.input_layout_desc.elements = elments;
        ps_cd.desc.input_layout_desc.elements_num = element_count;
        ps_cd.desc.rasterizer_state_desc.cull_mode = CullModeType::CULL_MODE_BACK;

        ShaderResourceCreateDesc SRVs[] = {
            {"cb_Renderer", {ShaderTypeFlags::SHADER_TYPE_VERTEX, ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_PerFrame", {ShaderTypeFlags::SHADER_TYPE_VERTEX, ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_PerDraw", {ShaderTypeFlags::SHADER_TYPE_VERTEX, ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_Renderer", {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_PerFrame", {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_PerDraw", {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_Material", {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_Light", {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"g_texture_material_albedo",
             {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_texture_material_roughness",
             {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_texture_material_metallic",
             {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_texture_material_normal",
             {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_texture_material_occlusion",
             {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_texture_material_emission",
             {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_texture_material_height",
             {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_texture_material_alpha_mask",
             {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_shadow_map_dir_light",
             {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_shadow_map_point_light",
             {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_shadow_map_spot_light",
             {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_sampler_comparison", {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_SAMPLER}},
            {"g_sampler_anisotropic",
             {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_SAMPLER}}};
        ps_cd.srv = SRVs;
        ps_cd.srv_num = (sizeof(SRVs) / sizeof(ShaderResourceCreateDesc));

        RendererSystem::Get().GetDevice()->CreatePipelineState(ps_cd, m_ambient_PSO);

        // Create lighting pass PSO

        std::shared_ptr<PPGEShader> lighting_ps;
        {
            auto &simple_ps = ShaderLibrary::Get().GetShaderCode("forward_pass_ps.hlsl");
            ShaderCreateDesc cd;
            cd.desc.shader_type_flags = ShaderTypeFlags::SHADER_TYPE_PIXEL;
            cd.compiler = ShaderCompilerType::SHADER_COMPILER_FXC;
            cd.file_path = "../../PPGE/renderer/shaders/forward_pass_ps.hlsl";
            cd.source_code = simple_ps.data.data();
            cd.source_code_size = simple_ps.data.size();
            cd.entry_point_name = "light_pass_PS";
            RendererSystem::Get().GetDevice()->CreateShader(cd, lighting_ps);
        }

        ps_cd.commited_ps = lighting_ps;
        ps_cd.desc.depth_stencil_desc.depth_write_enable = false;
        ps_cd.desc.depth_stencil_desc.depth_func = ComperisonFuncType::COMPARISON_FUNC_LESS_EQUAL;
        ps_cd.desc.blend_state_desc.render_target[0].blend_enable = true;
        ps_cd.desc.blend_state_desc.render_target[0].src_blend = BlendType::BLEND_ONE;
        ps_cd.desc.blend_state_desc.render_target[0].dst_blend = BlendType::BLEND_ONE;

        RendererSystem::Get().GetDevice()->CreatePipelineState(ps_cd, m_lighting_PSO);
    }

    // Create Shader Resource Binding
    m_SRB = m_ambient_PSO->CreateShaderResourceBinding();

    m_SRB->GetVariableByName("cb_PerDraw", ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(m_cb_per_draw);

    m_SRB->GetVariableByName("cb_PerDraw", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_cb_per_draw);
    m_SRB->GetVariableByName("cb_Material", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_cb_material);
    m_SRB->GetVariableByName("cb_Light", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_cb_light);

    m_SRB->GetVariableByName("g_sampler_comparison", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_comparison_sampler);
    m_SRB->GetVariableByName("g_sampler_anisotropic", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_anisotropic_sampler);
}

void ForwardRenderPass::Load(RenderGraph &render_graph)
{
    auto cb_renderer = render_graph.GetResource<PPGEBuffer>(CbRendererOptionsName);
    m_SRB->GetVariableByName("cb_Renderer", ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(cb_renderer);
    m_SRB->GetVariableByName("cb_Renderer", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(std::move(cb_renderer));

    auto cb_per_frame = render_graph.GetResource<PPGEBuffer>(CbPerFrameResourceName);
    m_SRB->GetVariableByName("cb_PerFrame", ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(cb_per_frame);
    m_SRB->GetVariableByName("cb_PerFrame", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(std::move(cb_per_frame));

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
    m_depth_buffer_dsv.reset();
    m_color_buffer_rtv.reset();
}

void ForwardRenderPass::Execute()
{
    RendererSystem::Get().GetImmediateContext()->SetPipelineStateObject(m_ambient_PSO);

    {
        std::shared_ptr<PPGETextureView> RTVs[] = {m_color_buffer_rtv};
        RendererSystem::Get().GetImmediateContext()->SetRenderTargets(1, RTVs, m_depth_buffer_dsv);

        auto text_desc = m_color_buffer_rtv->GetTexture()->GetDesc();
        Viewport viewport{.top_left_x = 0.0f,
                          .top_left_y = 0.0f,
                          .width = static_cast<float>(text_desc.width),
                          .height = static_cast<float>(text_desc.height),
                          .min_depth = 0.0f,
                          .max_depth = 1.0f};
        RendererSystem::Get().GetImmediateContext()->SetViewports(1, &viewport);
    }

    Draw();

    RendererSystem::Get().GetImmediateContext()->SetPipelineStateObject(m_lighting_PSO);

    const auto &data = GetSceneDataRef();
    for (auto [entity, light_transform_data, light_data] :
         data.scene.View<const TransformComponent, const LightComponent>().each())
    {
        // Update light buffer
        Light light(light_transform_data, light_data);
        CbLight *light_data_map;
        RendererSystem::Get().GetImmediateContext()->Map(m_cb_light.get(), MapType::MAP_WRITE, MapFlags::MAP_DISCARD,
                                                         reinterpret_cast<void **>(&light_data_map));
        if (light_data_map)
        {

            for (size_t i = 0; i < light.GetShadowMapViewCount(); i++)
            {
                auto view = light.GetLightView(i);
                auto proj = light.GetLightProj();
                auto view_proj = view * proj;
                light_data_map->view_proj[i] = view_proj.Transpose();
            }

            auto world = light_transform_data.GetWorldMatrix();
            light_data_map->position = world.Translation();
            light_data_map->direction = world.Backward();

            light_data_map->color_intensity =
                Math::Color(light_data.color.R(), light_data.color.G(), light_data.color.B(), light_data.intensity);

            light_data_map->light_options =
                light_data.CanCastShadow() ? LightOptions::CAN_CAST_SHADOW : LightOptions::NONE;

            switch (light_data.GetLightType())
            {
            case LightComponent::LightType::DIRECTIONAL: {
                light_data_map->light_options |= LightOptions::TYPE_DIRECTIONAL_LIGHT;
                if (light_data.CanCastShadow())
                    m_SRB->GetVariableByName("g_shadow_map_dir_light", ShaderTypeFlags::SHADER_TYPE_PIXEL)
                        ->Set(light.GetShadowMapSRView());
                break;
            }
            case LightComponent::LightType::POINT: {
                light_data_map->light_options |= LightOptions::TYPE_POINT_LIGHT;
                light_data_map->dist_attenuation_range =
                    Math::Vector4(light_data.dist_attenuation_a0, light_data.dist_attenuation_a1,
                                  light_data.dist_attenuation_a2, light_data.range);
                if (light_data.CanCastShadow())
                    m_SRB->GetVariableByName("g_shadow_map_point_light", ShaderTypeFlags::SHADER_TYPE_PIXEL)
                        ->Set(light.GetShadowMapSRView());
                break;
            }
            case LightComponent::LightType::SPOT: {
                light_data_map->light_options |= LightOptions::TYPE_SPOT_LIGHT;
                light_data_map->dist_attenuation_range =
                    Math::Vector4(light_data.dist_attenuation_a0, light_data.dist_attenuation_a1,
                                  light_data.dist_attenuation_a2, light_data.range);
                light_data_map->angle_attenuation = Math::Vector4(
                    light_data.spot_cutoff_angle, light_data.spot_inner_cone_angle, light_data.spot_decay_rate, 0.0f);
                if (light_data.CanCastShadow())
                    m_SRB->GetVariableByName("g_shadow_map_spot_light", ShaderTypeFlags::SHADER_TYPE_PIXEL)
                        ->Set(light.GetShadowMapSRView());
                break;
            };
            default: {
                PPGE_ASSERT(
                    false,
                    "Unknown light type. It should be set to either of directional, point and spot light types.");
                break;
            }
            }

            RendererSystem::Get().GetImmediateContext()->Unmap(m_cb_light.get());
        }

        Draw();
    }
}

void ForwardRenderPass::Draw()
{
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
            auto world_inv_trans = world.Invert().Transpose();

            per_draw_data->world = world.Transpose();
            per_draw_data->world_inverse_transpose = world_inv_trans.Transpose();

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