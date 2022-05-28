#include "shadow_pass.h"

#include "ecs/components/light_component.h"
#include "ecs/components/mesh_component.h"
#include "ecs/components/transform_components.h"
#include "ecs/scene.h"
#include "renderer/light.h"
#include "renderer/render_graph.h"
#include "renderer/render_pass_resource_descs.h"
#include "renderer/renderer_constants.h"
#include "renderer/renderer_types.h"
#include "renderer/shader_library.h"
#include "system/renderer_system.h"

namespace PPGE
{
ShadowPass::ShadowPass()
{
    // Create constant buffers
    {
        BufferDesc cb_desc;
        cb_desc.byte_width = sizeof(CbPerFrame);
        cb_desc.bind_flags = BindFlags::BIND_CONSTANT_BUFFER;
        cb_desc.usage = UsageType::USAGE_DYNAMIC;
        cb_desc.cpu_access_flags = CPUAccessFlags::CPU_ACCESS_WRITE;
        RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_per_frame);
        cb_desc.byte_width = sizeof(CbPerDraw);
        RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_per_draw);
    }

    // Create PSOs
    {
        // Create shadowpass PSO
        std::shared_ptr<PPGEShader> vs;
        {
            auto &simple_vs = ShaderLibrary::Get().GetShaderCode("shadow_pass.hlsl");
            ShaderCreateDesc cd;
            cd.desc.shader_type_flags = ShaderTypeFlags::SHADER_TYPE_VERTEX;
            cd.compiler = ShaderCompilerType::SHADER_COMPILER_FXC;
            cd.file_path = "../../PPGE/renderer/shaders/shadow_pass.hlsl";
            cd.source_code = simple_vs.data.data();
            cd.source_code_size = simple_vs.data.size();
            cd.entry_point_name = "main_VS";
            RendererSystem::Get().GetDevice()->CreateShader(cd, vs);
        }

        GfxPipelineStateCreateDesc ps_cd;

        ps_cd.commited_vs = vs;
        ps_cd.commited_ps = nullptr;

        auto [elments, element_count] = GetStandardLayout();
        ps_cd.desc.input_layout_desc.elements = elments;
        ps_cd.desc.input_layout_desc.elements_num = element_count;
        ps_cd.desc.rasterizer_state_desc.cull_mode = CullModeType::CULL_MODE_NONE;
        ps_cd.desc.rasterizer_state_desc.depth_bias = 0.005f;
        ps_cd.desc.rasterizer_state_desc.slope_scaled_depth_bias = 1.0f;

        ShaderResourceCreateDesc SRVs[] = {
            {"cb_Renderer", {ShaderTypeFlags::SHADER_TYPE_VERTEX, ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_PerFrame", {ShaderTypeFlags::SHADER_TYPE_VERTEX, ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_PerDraw", {ShaderTypeFlags::SHADER_TYPE_VERTEX, ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}}};
        ps_cd.srv = SRVs;
        ps_cd.srv_num = (sizeof(SRVs) / sizeof(ShaderResourceCreateDesc));

        RendererSystem::Get().GetDevice()->CreatePipelineState(ps_cd, m_PSO);
    }

    // Create Shader Resource Binding
    m_SRB = m_PSO->CreateShaderResourceBinding();
    m_SRB->GetVariableByName("cb_PerFrame", ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(m_cb_per_frame);
    m_SRB->GetVariableByName("cb_PerDraw", ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(m_cb_per_draw);
}

void ShadowPass::Load(RenderGraph &render_graph)
{
    auto cb_renderer = render_graph.GetResource<PPGEBuffer>(CbRendererOptionsName);
    m_SRB->GetVariableByName("cb_Renderer", ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(std::move(cb_renderer));
}

void ShadowPass::Unload()
{
}

void ShadowPass::Execute()
{
    RendererSystem::Get().GetImmediateContext()->SetPipelineStateObject(m_PSO);

    const auto &data = GetSceneDataRef();
    for (auto [entity, light_transform_data, light_data] :
         data.scene.View<const TransformComponent, const LightComponent>().each())
    {
        Light light(light_transform_data, light_data);

        for (size_t i = 0; i < light.GetShadowMapViewCount(); i++)
        {
            // Bind Depth Stencil Buffer
            auto shadow_map_dsv = light.GetShadowMapDSViewAt(i);
            RendererSystem::Get().GetImmediateContext()->ClearDepthStencil(shadow_map_dsv, 1.0f, 0);
            RendererSystem::Get().GetImmediateContext()->SetRenderTargets(0, nullptr, shadow_map_dsv);

            auto viewport = light.GetShadowMapViewportAt(i);
            RendererSystem::Get().GetImmediateContext()->SetViewports(1, &viewport);

            // Calculate Light View Proj
            CbPerFrame *map_data;
            {
                PPGE::RendererSystem::Get().GetImmediateContext()->Map(m_cb_per_frame.get(), PPGE::MapType::MAP_WRITE,
                                                                       PPGE::MapFlags::MAP_DISCARD,
                                                                       reinterpret_cast<void **>(&map_data));
                auto view = light.GetLightView(i);
                auto proj = light.GetLightProj();
                auto view_proj = view * proj;

                map_data->view = view.Transpose();
                map_data->view_inverse = view.Invert().Transpose();
                map_data->proj = proj.Transpose();
                map_data->proj_inverse = proj.Invert().Transpose();
                map_data->view_proj = view_proj.Transpose();
                map_data->view_proj_inverse = view_proj.Invert().Transpose();

                PPGE::RendererSystem::Get().GetImmediateContext()->Unmap(m_cb_per_frame.get());
            }

            // For each entity that can cast shadow draw
            for (auto [entity, obj_transform_data, mesh_filer_data, mesh_renderer_data] :
                 data.scene.View<const TransformComponent, const MeshFilterComponent, const MeshRendererComponent>()
                     .each())
            {
                // Bind vertex and index buffers
                {
                    std::shared_ptr<PPGEBuffer> vbs[] = {mesh_filer_data.vertex_buffer};
                    uint64_t offsets[] = {0};
                    RendererSystem::Get().GetImmediateContext()->SetVertexBuffers(1, vbs, offsets);
                    RendererSystem::Get().GetImmediateContext()->SetIndexBuffer(mesh_filer_data.index_buffer);
                }

                // Update per draw call constant buffer
                {
                    CbPerDraw *per_draw_data;
                    RendererSystem::Get().GetImmediateContext()->Map(m_cb_per_draw.get(), MapType::MAP_WRITE,
                                                                     MapFlags::MAP_DISCARD,
                                                                     reinterpret_cast<void **>(&per_draw_data));

                    auto world = obj_transform_data.GetWorldMatrix();
                    auto world_inv_trans = world.Invert().Transpose();

                    per_draw_data->world = world.Transpose();
                    per_draw_data->world_inverse_transpose = world_inv_trans.Transpose();

                    RendererSystem::Get().GetImmediateContext()->Unmap(m_cb_per_draw.get());
                }

                // Bind shader resources
                RendererSystem::Get().GetImmediateContext()->CommitShaderResources(m_SRB);

                // Issue indexed draw call
                RendererSystem::Get().GetImmediateContext()->DrawIndexed(mesh_filer_data.num_indices);
            }
        }
    }
}
} // namespace PPGE
