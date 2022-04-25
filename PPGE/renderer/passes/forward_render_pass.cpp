#include "forward_render_pass.h"

#include "ecs/components/light_component.h"
#include "ecs/components/mesh_component.h"
#include "ecs/components/transform_components.h"
#include "ecs/scene.h"
#include "renderer/camera_controller.h"
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
        cb_desc.byte_width = sizeof(CbLight);
        RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_light);
    }

    // Create Sampler
    {
        SamplerDesc sampler_desc;
        RendererSystem::Get().GetDevice()->CreateSampler(sampler_desc, m_sampler_state);
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
        std::shared_ptr<PPGE::PPGEShader> vs;
        {
            auto &simple_vs = ShaderLibrary::Get().GetShaderCode("forward_pass_vs.hlsl");
            PPGE::ShaderCreateDesc cd;
            cd.desc.shader_type_flags = PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX;
            cd.compiler = PPGE::ShaderCompilerType::SHADER_COMPILER_FXC;
            cd.file_path = "../../PPGE/renderer/shaders/forward_pass_vs.hlsl";
            cd.source_code = simple_vs.data.data();
            cd.source_code_size = simple_vs.data.size();
            cd.entry_point_name = "main_VS";
            PPGE::RendererSystem::Get().GetDevice()->CreateShader(cd, vs);
        }

        std::shared_ptr<PPGE::PPGEShader> ambient_ps;
        {
            auto &simple_ps = ShaderLibrary::Get().GetShaderCode("forward_pass_ps.hlsl");
            PPGE::ShaderCreateDesc cd;
            cd.desc.shader_type_flags = PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL;
            cd.compiler = PPGE::ShaderCompilerType::SHADER_COMPILER_FXC;
            cd.file_path = "../../PPGE/renderer/shaders/forward_pass_ps.hlsl";
            cd.source_code = simple_ps.data.data();
            cd.source_code_size = simple_ps.data.size();
            cd.entry_point_name = "ambient_pass_PS";
            PPGE::RendererSystem::Get().GetDevice()->CreateShader(cd, ambient_ps);
        }

        PPGE::GfxPipelineStateCreateDesc ps_cd;

        ps_cd.commited_vs = vs;
        ps_cd.commited_ps = ambient_ps;

        ps_cd.desc.input_layout_desc.elements = GetLayout(StandardLayout);
        ps_cd.desc.input_layout_desc.elements_num = 8;
        ps_cd.desc.rasterizer_state_desc.cull_mode = PPGE::CullModeType::CULL_MODE_NONE;

        PPGE::ShaderResourceCreateDesc SRVs[] = {
            {"cb_PerFrame",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX, PPGE::ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_PerDraw",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX, PPGE::ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_PerFrame",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_PerDraw",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_Light",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"g_texture_material_albedo",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_texture_material_specular",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_texture_material_normal",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_sampler_anisotropic_wrap",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_SAMPLER}}};
        ps_cd.srv = SRVs;
        ps_cd.srv_num = (sizeof(SRVs) / sizeof(PPGE::ShaderResourceCreateDesc));

        PPGE::RendererSystem::Get().GetDevice()->CreatePipelineState(ps_cd, m_ambient_PSO);

        // Create lighting pass PSO

        std::shared_ptr<PPGE::PPGEShader> lighting_ps;
        {
            auto &simple_ps = ShaderLibrary::Get().GetShaderCode("forward_pass_ps.hlsl");
            PPGE::ShaderCreateDesc cd;
            cd.desc.shader_type_flags = PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL;
            cd.compiler = PPGE::ShaderCompilerType::SHADER_COMPILER_FXC;
            cd.file_path = "../../PPGE/renderer/shaders/forward_pass_ps.hlsl";
            cd.source_code = simple_ps.data.data();
            cd.source_code_size = simple_ps.data.size();
            cd.entry_point_name = "light_pass_PS";
            PPGE::RendererSystem::Get().GetDevice()->CreateShader(cd, lighting_ps);
        }

        ps_cd.commited_ps = lighting_ps;
        ps_cd.desc.depth_stencil_desc.depth_write_enable = false;
        ps_cd.desc.depth_stencil_desc.depth_func = ComperisonFuncType::COMPARISON_FUNC_LESS_EQUAL;
        ps_cd.desc.blend_state_desc.render_target[0].blend_enable = true;
        ps_cd.desc.blend_state_desc.render_target[0].src_blend = BlendType::BLEND_ONE;
        ps_cd.desc.blend_state_desc.render_target[0].dst_blend = BlendType::BLEND_ONE;

        PPGE::RendererSystem::Get().GetDevice()->CreatePipelineState(ps_cd, m_lighting_PSO);
    }

    // Create Shader Resource Binding
    m_SRB = m_ambient_PSO->CreateShaderResourceBinding();

    m_SRB->GetVariableByName("cb_PerDraw", PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(m_cb_per_draw);

    m_SRB->GetVariableByName("cb_PerDraw", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_cb_per_draw);
    m_SRB->GetVariableByName("cb_Light", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_cb_light);

    m_SRB->GetVariableByName("g_sampler_anisotropic_wrap", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)
        ->Set(m_sampler_state);
}

void ForwardRenderPass::Load(RenderGraph &render_graph)
{
    auto cb_per_frame = render_graph.GetResource<PPGEBuffer>(CbPerFrameResourceName);
    m_SRB->GetVariableByName("cb_PerFrame", PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(cb_per_frame);
    m_SRB->GetVariableByName("cb_PerFrame", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(std::move(cb_per_frame));

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
    RendererSystem::Get().GetImmediateContext()->SetPipelineStateObject(m_ambient_PSO);

    Draw();

    RendererSystem::Get().GetImmediateContext()->SetPipelineStateObject(m_lighting_PSO);

    const auto &data = GetSceneDataRef();
    for (auto [entity, transform, light] : data.scene.View<const TransformComponent, const LightComponent>().each())
    {
        // Update light buffer
        {
            CbLight *light_data;
            PPGE::RendererSystem::Get().GetImmediateContext()->Map(m_cb_light.get(), PPGE::MapType::MAP_WRITE,
                                                                   PPGE::MapFlags::MAP_DISCARD,
                                                                   reinterpret_cast<void **>(&light_data));
            auto world = transform.GetWorldMatrix();

            light_data->position = world.Translation();
            light_data->direction = world.Forward();

            light_data->color_intensity =
                Math::Color(light.color.R(), light.color.G(), light.color.B(), light.intensity);

            light_data->light_options = LightOptions::NONE;
            switch (light.type)
            {
            case LightComponent::LightType::DIRECTIONAL: {
                light_data->light_options |= LightOptions::TYPE_DIRECTIONAL_LIGHT;
                break;
            }
            case LightComponent::LightType::POINT: {
                light_data->light_options |= LightOptions::TYPE_POINT_LIGHT;
                light_data->dist_attenuation_range = Math::Vector4(light.dist_attenuation_a0, light.dist_attenuation_a1,
                                                                   light.dist_attenuation_a2, light.range);
                break;
            }
            case LightComponent::LightType::SPOT: {
                light_data->light_options |= LightOptions::TYPE_SPOT_LIGHT;
                light_data->dist_attenuation_range = Math::Vector4(light.dist_attenuation_a0, light.dist_attenuation_a1,
                                                                   light.dist_attenuation_a2, light.range);
                light_data->angle_attenuation =
                    Math::Vector4(light.spot_cutoff_angle, light.spot_inner_cone_angle, light.spot_decay_rate, 0.0f);
                break;
            };
            default: {
                PPGE_ASSERT(
                    false,
                    "Unknown light type. It should be set to either of directional, point and spot light types.");
                break;
            }
            }

            PPGE::RendererSystem::Get().GetImmediateContext()->Unmap(m_cb_light.get());
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
        {
            std::shared_ptr<PPGE::PPGEBuffer> vbs[] = {mesh_filer.vertex_buffer};
            uint64_t offsets[] = {0};
            PPGE::RendererSystem::Get().GetImmediateContext()->SetVertexBuffers(1, vbs, offsets);
            PPGE::RendererSystem::Get().GetImmediateContext()->SetIndexBuffer(mesh_filer.index_buffer);
        }

        // Update per draw call constant buffer
        {
            CbPerDraw *per_draw_data;
            PPGE::RendererSystem::Get().GetImmediateContext()->Map(m_cb_per_draw.get(), PPGE::MapType::MAP_WRITE,
                                                                   PPGE::MapFlags::MAP_DISCARD,
                                                                   reinterpret_cast<void **>(&per_draw_data));
            per_draw_data->material_options = MaterialOptions::NONE;
            per_draw_data->entity_id = static_cast<uint32_t>(entity);
            per_draw_data->albedo_color = mesh_renderer.albedo_color;
            per_draw_data->specular_color = mesh_renderer.specular_color;

            // Set per object shader resources
            {
                if (mesh_renderer.albedo_map)
                {
                    m_SRB->GetVariableByName("g_texture_material_albedo", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)
                        ->Set(mesh_renderer.albedo_map);
                    per_draw_data->material_options |= MaterialOptions::ALBEDO_MAP_BOUND;
                }

                if (mesh_renderer.specular_map)
                {
                    m_SRB->GetVariableByName("g_texture_material_specular", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)
                        ->Set(mesh_renderer.specular_map);
                    per_draw_data->material_options |= MaterialOptions::SPECULAR_MAP_BOUND;
                }

                if (mesh_renderer.normal_map)
                {
                    m_SRB->GetVariableByName("g_texture_material_normal", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)
                        ->Set(mesh_renderer.normal_map);
                    per_draw_data->material_options |= MaterialOptions::NORMAL_MAP_BOUND;
                }
            }

            auto world = transform.GetWorldMatrix();
            auto world_inv_trans = world.Invert().Transpose();

            per_draw_data->world = world.Transpose();
            per_draw_data->world_inverse_transpose = world_inv_trans.Transpose();

            PPGE::RendererSystem::Get().GetImmediateContext()->Unmap(m_cb_per_draw.get());
        }

        // Bind shader resources
        PPGE::RendererSystem::Get().GetImmediateContext()->CommitShaderResources(m_SRB);

        // Issue indexed draw call
        PPGE::RendererSystem::Get().GetImmediateContext()->DrawIndexed(mesh_filer.num_indices);
    }
}
} // namespace PPGE