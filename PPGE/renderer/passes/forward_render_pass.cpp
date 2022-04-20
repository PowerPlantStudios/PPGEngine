#include "forward_render_pass.h"

#include "ecs/components/mesh_component.h"
#include "ecs/components/transform_components.h"
#include "ecs/scene.h"
#include "renderer/render_graph.h"
#include "renderer/renderer_constants.h"
#include "renderer/renderer_types.h"
#include "renderer/shader_library.h"
#include "renderer/camera_controller.h"
#include "system/renderer_system.h"

namespace PPGE
{
struct alignas(16) cbPerObjectVS
{
    Math::Matrix model;
    Math::Matrix modelInvTran;
};

struct alignas(16) cbPerFramePS
{
    Math::Vector3 eyePos;
};

struct alignas(16) cbPerObjectPS
{
    Math::Color albedo;
    Math::Color specular;
    bool albedo_map_bound;
    bool specular_map_bound;
    bool normal_map_bound;
};

ForwardRenderPass::ForwardRenderPass()
{
    // Create constant buffers
    {
        BufferDesc cb_desc;
        cb_desc.byte_width = sizeof(cbPerObjectVS);
        cb_desc.bind_flags = BindFlags::BIND_CONSTANT_BUFFER;
        cb_desc.usage = UsageType::USAGE_DYNAMIC;
        cb_desc.cpu_access_flags = CPUAccessFlags::CPU_ACCESS_WRITE;
        RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_per_object_vs);
        cb_desc.byte_width = sizeof(cbPerFramePS);
        RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_per_frame_ps);
        cb_desc.byte_width = sizeof(cbPerObjectPS);
        RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_per_object_ps);
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

    // Create Pipeline State Object
    {
        std::shared_ptr<PPGE::PPGEShader> vs;
        {
            auto &simple_vs = ShaderLibrary::Get().GetShaderCode("forward_pass.hlsl");
            PPGE::ShaderCreateDesc cd;
            cd.desc.shader_type_flags = PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX;
            cd.compiler = PPGE::ShaderCompilerType::SHADER_COMPILER_FXC;
            cd.source_code = simple_vs.data.data();
            cd.source_code_size = simple_vs.data.size();
            cd.entry_point_name = "main_VS";
            PPGE::RendererSystem::Get().GetDevice()->CreateShader(cd, vs);
        }

        std::shared_ptr<PPGE::PPGEShader> ps;
        {
            auto &simple_ps = ShaderLibrary::Get().GetShaderCode("forward_pass.hlsl");
            PPGE::ShaderCreateDesc cd;
            cd.desc.shader_type_flags = PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL;
            cd.compiler = PPGE::ShaderCompilerType::SHADER_COMPILER_FXC;
            cd.source_code = simple_ps.data.data();
            cd.source_code_size = simple_ps.data.size();
            cd.entry_point_name = "main_PS";
            PPGE::RendererSystem::Get().GetDevice()->CreateShader(cd, ps);
        }

        PPGE::GfxPipelineStateCreateDesc ps_cd;
        ps_cd.desc.input_layout_desc.elements = GetLayout(FullLayout);
        ps_cd.desc.input_layout_desc.elements_num = 8;
        ps_cd.desc.rasterizer_state_desc.cull_mode = PPGE::CullModeType::CULL_MODE_NONE;
        ps_cd.commited_vs = vs;
        ps_cd.commited_ps = ps;

        PPGE::ShaderResourceCreateDesc SRVs[] = {
            {"cb_PerFrame",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX, PPGE::ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_PerObject",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX, PPGE::ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_View",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"cb_Color",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
            {"g_albedo",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_specular",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_normal",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_sampler",
             {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_SAMPLER}}};
        ps_cd.srv = SRVs;
        ps_cd.srv_num = (sizeof(SRVs) / sizeof(PPGE::ShaderResourceCreateDesc));

        PPGE::RendererSystem::Get().GetDevice()->CreatePipelineState(ps_cd, m_PSO);
    }

    // Create Shader Resource Binding
    m_SRB = m_PSO->CreateShaderResourceBinding();
    m_SRB->GetVariableByName("cb_PerObject", PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(m_cb_per_object_vs);
    m_SRB->GetVariableByName("cb_View", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_cb_per_frame_ps);
    m_SRB->GetVariableByName("cb_Color", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_cb_per_object_ps);
    m_SRB->GetVariableByName("g_sampler", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_sampler_state);
}

void ForwardRenderPass::Load(RenderGraph &render_graph)
{
    auto cb_per_frame = render_graph.GetResource<PPGEBuffer>(CbCameraDataResourceName);
    m_SRB->GetVariableByName("cb_PerFrame", PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(std::move(cb_per_frame));

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

    // Update per frame PS constant buffer
    {
        cbPerFramePS *map_data;
        PPGE::RendererSystem::Get().GetImmediateContext()->Map(m_cb_per_frame_ps.get(), PPGE::MapType::MAP_WRITE,
                                                               PPGE::MapFlags::MAP_DISCARD,
                                                               reinterpret_cast<void **>(&map_data));
        *map_data = cbPerFramePS{.eyePos = data.active_camera.GetPosition()};
        PPGE::RendererSystem::Get().GetImmediateContext()->Unmap(m_cb_per_frame_ps.get());
    }

    RendererSystem::Get().GetImmediateContext()->SetPipelineStateObject(m_PSO);

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

        // Update per object VS constant buffer
        {
            cbPerObjectVS *map_data;
            PPGE::RendererSystem::Get().GetImmediateContext()->Map(m_cb_per_object_vs.get(), PPGE::MapType::MAP_WRITE,
                                                                   PPGE::MapFlags::MAP_DISCARD,
                                                                   reinterpret_cast<void **>(&map_data));
            auto model = transform.GetModelMatrix();
            auto inv_mode = model.Invert();
            *map_data = cbPerObjectVS{.model = model.Transpose(), .modelInvTran = inv_mode.Transpose()};
            PPGE::RendererSystem::Get().GetImmediateContext()->Unmap(m_cb_per_object_vs.get());
        }

        // Set per object shader resources
        {
            cbPerObjectPS *map_data;
            PPGE::RendererSystem::Get().GetImmediateContext()->Map(m_cb_per_object_ps.get(), PPGE::MapType::MAP_WRITE,
                                                                   PPGE::MapFlags::MAP_DISCARD,
                                                                   reinterpret_cast<void **>(&map_data));
            if (mesh_renderer.albedo_map)
            {
                m_SRB->GetVariableByName("g_albedo", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)
                    ->Set(mesh_renderer.albedo_map);
                map_data->albedo_map_bound = true;
            }
            else
            {
                map_data->albedo_map_bound = false;
                map_data->albedo = mesh_renderer.albedo_color;
            }

            if (mesh_renderer.specular_map)
            {
                m_SRB->GetVariableByName("g_specular", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)
                    ->Set(mesh_renderer.specular_map);
                map_data->specular_map_bound = true;
            }
            else
            {
                map_data->specular_map_bound = false;
                map_data->specular = mesh_renderer.specular_color;
            }

            if (mesh_renderer.normal_map)
            {
                m_SRB->GetVariableByName("g_normal", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)
                    ->Set(mesh_renderer.normal_map);
                map_data->normal_map_bound = true;
            }
            else
            {
                map_data->normal_map_bound = false;
            }

            PPGE::RendererSystem::Get().GetImmediateContext()->Unmap(m_cb_per_object_ps.get());
        }
        PPGE::RendererSystem::Get().GetImmediateContext()->CommitShaderResources(m_SRB);

        // Draw indexed
        PPGE::RendererSystem::Get().GetImmediateContext()->DrawIndexed(mesh_filer.num_indices);
    }
}
} // namespace PPGE