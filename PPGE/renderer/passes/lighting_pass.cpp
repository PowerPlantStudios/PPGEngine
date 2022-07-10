#include "lighting_pass.h"

#include "ecs/components/light_component.h"
#include "ecs/components/mesh_component.h"
#include "ecs/components/transform_components.h"
#include "ecs/scene.h"
#include "renderer/light.h"
#include "renderer/render_graph.h"
#include "renderer/renderer_constants.h"
#include "renderer/renderer_types.h"
#include "renderer/shader_library.h"
#include "system/renderer_system.h"

namespace PPGE
{
LightingPass::LightingPass()
{
    // Create constant buffers
    {
        BufferDesc cb_desc;
        cb_desc.bind_flags = BindFlags::BIND_CONSTANT_BUFFER;
        cb_desc.usage = UsageType::USAGE_DYNAMIC;
        cb_desc.cpu_access_flags = CPUAccessFlags::CPU_ACCESS_WRITE;
        cb_desc.byte_width = sizeof(CbLight);
        RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_light);
    }
    // Create Anisotropic Sampler
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
    // Create Comparison Sampler
    {
        SamplerDesc sampler_desc;
        sampler_desc.min_filter = FilterType::FILTER_COMPARISON_LINEAR;
        sampler_desc.mag_filter = FilterType::FILTER_COMPARISON_LINEAR;
        sampler_desc.mip_filter = FilterType::FILTER_COMPARISON_POINT;
        sampler_desc.comperison_func = ComperisonFuncType::COMPARISON_FUNC_LESS;
        RendererSystem::Get().GetDevice()->CreateSampler(sampler_desc, m_comparison_sampler);
    }
    // Create PSOs
    {
        // Create VS
        std::shared_ptr<PPGEShader> vs;
        {
            auto &screen_quad_vs = ShaderLibrary::Get().GetShaderCode("screen_quad_vs.hlsl");
            ShaderCreateDesc cd;
            cd.desc.shader_type_flags = ShaderTypeFlags::SHADER_TYPE_VERTEX;
            cd.compiler = ShaderCompilerType::SHADER_COMPILER_FXC;
            cd.source_code = screen_quad_vs.data.data();
            cd.source_code_size = screen_quad_vs.data.size();
            cd.entry_point_name = "main_VS";
            RendererSystem::Get().GetDevice()->CreateShader(cd, vs);
        }
        // Create PS
        std::shared_ptr<PPGEShader> lighting_ps;
        {
            auto &lighing_ps_code = ShaderLibrary::Get().GetShaderCode("lighting_pass_ps.hlsl");
            ShaderCreateDesc cd;
            cd.desc.shader_type_flags = ShaderTypeFlags::SHADER_TYPE_PIXEL;
            cd.compiler = ShaderCompilerType::SHADER_COMPILER_FXC;
            cd.file_path = "../../PPGE/renderer/shaders/lighting_pass_ps.hlsl";
            cd.source_code = lighing_ps_code.data.data();
            cd.source_code_size = lighing_ps_code.data.size();
            cd.entry_point_name = "light_pass_PS";
            RendererSystem::Get().GetDevice()->CreateShader(cd, lighting_ps);
        }

        GfxPipelineStateCreateDesc ps_cd;

        ps_cd.commited_vs = vs;
        ps_cd.commited_ps = lighting_ps;

        const char *ps_cbs_1[] = {"cb_renderer", "cb_per_frame"};
        const char *ps_cbs_2[] = {"cb_light"};
        const char *ps_light_srvs_1[] = {"g_buffer_albedo", "g_buffer_normal", "g_buffer_position",
                                         "g_buffer_material"};
        const char *ps_light_srvs_2[] = {"g_shadow_map_dir_light", "g_shadow_map_point_light",
                                         "g_shadow_map_spot_light"};
        const char *ps_smps[] = {"g_sampler_comparison", "g_sampler_anisotropic"};

        ShaderResourceRangeCreateDesc range_light_pass[] = {
            {{ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER, 0u},
             sizeof(ps_cbs_1) / sizeof(ShaderResourceDesc),
             ps_cbs_1},
            {{ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER, 4u},
             sizeof(ps_cbs_2) / sizeof(ShaderResourceDesc),
             ps_cbs_2},
            {{ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV, 8u},
             sizeof(ps_light_srvs_1) / sizeof(ShaderResourceDesc),
             ps_light_srvs_1},
            {{ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV, 13u},
             sizeof(ps_light_srvs_2) / sizeof(ShaderResourceDesc),
             ps_light_srvs_2},
            {{ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_SAMPLER, 0u},
             sizeof(ps_smps) / sizeof(ShaderResourceDesc),
             ps_smps},
        };

        ps_cd.sr_create_desc.range = range_light_pass;
        ps_cd.sr_create_desc.range_num = (sizeof(range_light_pass) / sizeof(ShaderResourceRangeCreateDesc));

        ps_cd.desc.depth_stencil_desc.depth_test_enable = false;
        ps_cd.desc.depth_stencil_desc.depth_write_enable = false;
        ps_cd.desc.blend_state_desc.render_target[0].blend_enable = true;
        ps_cd.desc.blend_state_desc.render_target[0].src_blend = BlendType::BLEND_ONE;
        ps_cd.desc.blend_state_desc.render_target[0].dst_blend = BlendType::BLEND_ONE;
        ps_cd.desc.rasterizer_state_desc.cull_mode = CullModeType::CULL_MODE_NONE;
        ps_cd.desc.primitive_topology = PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_TRIANGLELIST;

        RendererSystem::Get().GetDevice()->CreatePipelineState(ps_cd, m_light_PSO);

        // Create PS
        std::shared_ptr<PPGEShader> emission_ps;
        {
            auto &emission_ps_code = ShaderLibrary::Get().GetShaderCode("lighting_pass_ps.hlsl");
            ShaderCreateDesc cd;
            cd.desc.shader_type_flags = ShaderTypeFlags::SHADER_TYPE_PIXEL;
            cd.compiler = ShaderCompilerType::SHADER_COMPILER_FXC;
            cd.file_path = "../../PPGE/renderer/shaders/lighting_pass_ps.hlsl";
            cd.source_code = emission_ps_code.data.data();
            cd.source_code_size = emission_ps_code.data.size();
            cd.entry_point_name = "emission_PS";
            RendererSystem::Get().GetDevice()->CreateShader(cd, emission_ps);
        }

        ps_cd.commited_ps = emission_ps;

        const char *ps_emission_srvs[] = {"g_buffer_emission"};

        ShaderResourceRangeCreateDesc range_emission_pass[] = {
            {{ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV, 12u},
             sizeof(ps_emission_srvs) / sizeof(ShaderResourceDesc),
             ps_emission_srvs},
            {{ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_SAMPLER, 0u},
             sizeof(ps_smps) / sizeof(ShaderResourceDesc),
             ps_smps},
        };

        ps_cd.sr_create_desc.range = range_emission_pass;
        ps_cd.sr_create_desc.range_num = (sizeof(range_emission_pass) / sizeof(ShaderResourceRangeCreateDesc));

        RendererSystem::Get().GetDevice()->CreatePipelineState(ps_cd, m_emission_PSO);
    }
    // Create Shader Resource Binding
    m_light_SRB = m_light_PSO->CreateShaderResourceBinding();
    {
        m_light_SRB->GetVariableByName("cb_light", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_cb_light);
        m_light_SRB->GetVariableByName("g_sampler_comparison", ShaderTypeFlags::SHADER_TYPE_PIXEL)
            ->Set(m_comparison_sampler);
        m_light_SRB->GetVariableByName("g_sampler_anisotropic", ShaderTypeFlags::SHADER_TYPE_PIXEL)
            ->Set(m_anisotropic_sampler);
    }
    m_emission_SRB = m_emission_PSO->CreateShaderResourceBinding();
    {
        m_emission_SRB->GetVariableByName("g_sampler_comparison", ShaderTypeFlags::SHADER_TYPE_PIXEL)
            ->Set(m_comparison_sampler);
        m_emission_SRB->GetVariableByName("g_sampler_anisotropic", ShaderTypeFlags::SHADER_TYPE_PIXEL)
            ->Set(m_anisotropic_sampler);
    }
}

void LightingPass::Load(RenderGraph &render_graph)
{
    auto cb_renderer = render_graph.GetResource<PPGEBuffer>(CbRendererOptionsName);
    {
        m_light_SRB->GetVariableByName("cb_renderer", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(std::move(cb_renderer));
    }

    auto cb_per_frame = render_graph.GetResource<PPGEBuffer>(CbPerFrameResourceName);
    {
        m_light_SRB->GetVariableByName("cb_per_frame", ShaderTypeFlags::SHADER_TYPE_PIXEL)
            ->Set(std::move(cb_per_frame));
    }

    auto albedo_buffer = render_graph.GetResource<PPGETexture>(RenderPassResourceDescs::Albedo_Buffer_Resource);
    {
        TextureViewDesc desc;
        desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE;
        desc.resource_dimension = albedo_buffer->GetDesc().resource_dimension;
        desc.format = albedo_buffer->GetDesc().format_type;
        desc.most_detailed_mip = 0;
        desc.mip_levels_num = 1;
        desc.array_slices_num = albedo_buffer->GetDesc().array_size;
        desc.first_array_slice = 0;
        m_albedo_buffer_srv = albedo_buffer->CreateView(desc);
        m_light_SRB->GetVariableByName("g_buffer_albedo", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_albedo_buffer_srv);
    }

    auto normal_buffer = render_graph.GetResource<PPGETexture>(RenderPassResourceDescs::Normal_Buffer_Resource);
    {
        TextureViewDesc desc;
        desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE;
        desc.resource_dimension = normal_buffer->GetDesc().resource_dimension;
        desc.format = normal_buffer->GetDesc().format_type;
        desc.most_detailed_mip = 0;
        desc.mip_levels_num = 1;
        desc.array_slices_num = normal_buffer->GetDesc().array_size;
        desc.first_array_slice = 0;
        m_normal_buffer_srv = normal_buffer->CreateView(desc);
        m_light_SRB->GetVariableByName("g_buffer_normal", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_normal_buffer_srv);
    }

    auto position_buffer = render_graph.GetResource<PPGETexture>(RenderPassResourceDescs::Position_Buffer_Resource);
    {
        TextureViewDesc desc;
        desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE;
        desc.resource_dimension = position_buffer->GetDesc().resource_dimension;
        desc.format = position_buffer->GetDesc().format_type;
        desc.most_detailed_mip = 0;
        desc.mip_levels_num = 1;
        desc.array_slices_num = position_buffer->GetDesc().array_size;
        desc.first_array_slice = 0;
        m_position_buffer_srv = position_buffer->CreateView(desc);
        m_light_SRB->GetVariableByName("g_buffer_position", ShaderTypeFlags::SHADER_TYPE_PIXEL)
            ->Set(m_position_buffer_srv);
    }

    auto material_buffer = render_graph.GetResource<PPGETexture>(RenderPassResourceDescs::Material_Buffer_Resource);
    {
        TextureViewDesc desc;
        desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE;
        desc.resource_dimension = material_buffer->GetDesc().resource_dimension;
        desc.format = material_buffer->GetDesc().format_type;
        desc.most_detailed_mip = 0;
        desc.mip_levels_num = 1;
        desc.array_slices_num = material_buffer->GetDesc().array_size;
        desc.first_array_slice = 0;
        m_material_buffer_srv = material_buffer->CreateView(desc);
        m_light_SRB->GetVariableByName("g_buffer_material", ShaderTypeFlags::SHADER_TYPE_PIXEL)
            ->Set(m_material_buffer_srv);
    }

    auto emission_buffer = render_graph.GetResource<PPGETexture>(RenderPassResourceDescs::Emission_Buffer_Resource);
    {
        TextureViewDesc desc;
        desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE;
        desc.resource_dimension = emission_buffer->GetDesc().resource_dimension;
        desc.format = emission_buffer->GetDesc().format_type;
        desc.most_detailed_mip = 0;
        desc.mip_levels_num = 1;
        desc.array_slices_num = emission_buffer->GetDesc().array_size;
        desc.first_array_slice = 0;
        m_emission_buffer_srv = emission_buffer->CreateView(desc);
        m_emission_SRB->GetVariableByName("g_buffer_emission", ShaderTypeFlags::SHADER_TYPE_PIXEL)
            ->Set(m_emission_buffer_srv);
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

void LightingPass::Unload()
{
    m_albedo_buffer_srv.reset();
    m_normal_buffer_srv.reset();
    m_position_buffer_srv.reset();
    m_material_buffer_srv.reset();
    m_emission_buffer_srv.reset();
    m_color_buffer_rtv.reset();
}

void LightingPass::Execute()
{
    // Bind render targets and viewports
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
    // Bind lighting PSO
    RendererSystem::Get().GetImmediateContext()->SetPipelineStateObject(m_light_PSO);
    // Illuminate the scene per light w/ G-Buffer
    const auto &data = GetSceneDataRef();
    for (auto [entity, light_transform_data, light_data] :
         data.scene.View<const TransformComponent, const LightComponent>().each())
    {
        // Update light buffer and shadow maps
        Light light(light_transform_data, light_data);
        {
            CbLight *light_data_map;
            RendererSystem::Get().GetImmediateContext()->Map(m_cb_light.get(), MapType::MAP_WRITE,
                                                             MapFlags::MAP_DISCARD,
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
                        m_light_SRB->GetVariableByName("g_shadow_map_dir_light", ShaderTypeFlags::SHADER_TYPE_PIXEL)
                            ->Set(light.GetShadowMapSRView());
                    break;
                }
                case LightComponent::LightType::POINT: {
                    light_data_map->light_options |= LightOptions::TYPE_POINT_LIGHT;
                    light_data_map->dist_attenuation_range =
                        Math::Vector4(light_data.dist_attenuation_a0, light_data.dist_attenuation_a1,
                                      light_data.dist_attenuation_a2, light_data.range);
                    if (light_data.CanCastShadow())
                        m_light_SRB->GetVariableByName("g_shadow_map_point_light", ShaderTypeFlags::SHADER_TYPE_PIXEL)
                            ->Set(light.GetShadowMapSRView());
                    break;
                }
                case LightComponent::LightType::SPOT: {
                    light_data_map->light_options |= LightOptions::TYPE_SPOT_LIGHT;
                    light_data_map->dist_attenuation_range =
                        Math::Vector4(light_data.dist_attenuation_a0, light_data.dist_attenuation_a1,
                                      light_data.dist_attenuation_a2, light_data.range);
                    light_data_map->angle_attenuation =
                        Math::Vector4(light_data.spot_cutoff_angle, light_data.spot_inner_cone_angle,
                                      light_data.spot_decay_rate, 0.0f);
                    if (light_data.CanCastShadow())
                        m_light_SRB->GetVariableByName("g_shadow_map_spot_light", ShaderTypeFlags::SHADER_TYPE_PIXEL)
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
        }
        // Bind shader resources
        RendererSystem::Get().GetImmediateContext()->CommitShaderResources(m_light_SRB);
        // Render SS Quad
        PPGE::RendererSystem::Get().GetImmediateContext()->Draw(3);
    }
    // Bind emission PSO and draw emission sources
    RendererSystem::Get().GetImmediateContext()->SetPipelineStateObject(m_emission_PSO);
    // Bind shader resources
    RendererSystem::Get().GetImmediateContext()->CommitShaderResources(m_emission_SRB);
    // Render SS Quad
    PPGE::RendererSystem::Get().GetImmediateContext()->Draw(3);
}
} // namespace PPGE
