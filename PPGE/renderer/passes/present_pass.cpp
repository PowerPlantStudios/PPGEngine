#include "present_pass.h"

#include "renderer/render_graph.h"
#include "renderer/render_pass_resource_descs.h"
#include "renderer/shader_library.h"
#include "system/renderer_system.h"

namespace PPGE
{
PresentPass::PresentPass()
{
    // Create Sampler
    {
        SamplerDesc sampler_desc;
        RendererSystem::Get().GetDevice()->CreateSampler(sampler_desc, m_sampler_state);
    }

    // Create PSOs
    {
        // Create ambient pass PSO
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

        std::shared_ptr<PPGEShader> ps;
        {
            auto &present_ps = ShaderLibrary::Get().GetShaderCode("present_ps.hlsl");
            ShaderCreateDesc cd;
            cd.desc.shader_type_flags = ShaderTypeFlags::SHADER_TYPE_PIXEL;
            cd.compiler = ShaderCompilerType::SHADER_COMPILER_FXC;
            cd.file_path = "../../PPGE/renderer/shaders/present_ps.hlsl";
            cd.source_code = present_ps.data.data();
            cd.source_code_size = present_ps.data.size();
            cd.entry_point_name = "main_PS";
            RendererSystem::Get().GetDevice()->CreateShader(cd, ps);
        }

        GfxPipelineStateCreateDesc ps_cd;

        ps_cd.commited_vs = vs;
        ps_cd.commited_ps = ps;
        ps_cd.desc.rasterizer_state_desc.cull_mode = CullModeType::CULL_MODE_NONE;
        ps_cd.desc.primitive_topology = PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_TRIANGLELIST;

        ShaderResourceCreateDesc SRVs[] = {
            {"g_color_buffer", {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
            {"g_sampler", {ShaderTypeFlags::SHADER_TYPE_PIXEL, ShaderResourceType::SHADER_RESOURCE_SAMPLER}}};
        ps_cd.srv = SRVs;
        ps_cd.srv_num = (sizeof(SRVs) / sizeof(ShaderResourceCreateDesc));

        RendererSystem::Get().GetDevice()->CreatePipelineState(ps_cd, m_PSO);
    }

    // Create Shader Resource Binding
    m_SRB = m_PSO->CreateShaderResourceBinding();
    m_SRB->GetVariableByName("g_sampler", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_sampler_state);
}

void PresentPass::Load(RenderGraph &render_graph)
{
    auto color_buffer = render_graph.GetResource<PPGETexture>(RenderPassResourceDescs::Color_Buffer_Resource);
    {
        TextureViewDesc desc;
        desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE;
        desc.resource_dimension = color_buffer->GetDesc().resource_dimension;
        desc.format = color_buffer->GetDesc().format_type;
        desc.most_detailed_mip = 0;
        desc.mip_levels_num = 1;
        desc.array_slices_num = color_buffer->GetDesc().array_size;
        desc.first_array_slice = 0;
        auto color_buffer_srv = color_buffer->CreateView(desc);
        m_SRB->GetVariableByName("g_color_buffer", ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(std::move(color_buffer_srv));
    }
}

void PresentPass::Unload()
{
}

void PresentPass::Execute()
{
    RendererSystem::Get().GetImmediateContext()->SetPipelineStateObject(m_PSO);

    {
        std::shared_ptr<PPGETextureView> RTVs[] = {RendererSystem::Get().GetSwapChain()->GetBackBufferRTV()};
        std::shared_ptr<PPGETextureView> DSV = RendererSystem::Get().GetSwapChain()->GetDepthBufferDSV();
        RendererSystem::Get().GetImmediateContext()->ClearRenderTarget(RTVs[0], 0.0f, 0.0f, 0.0f, 0.0f);
        RendererSystem::Get().GetImmediateContext()->ClearDepthStencil(DSV, 1.0f, 0);
        auto text_desc = RTVs[0]->GetTexture()->GetDesc();
        Viewport viewport{.top_left_x = 0.0f,
                          .top_left_y = 0.0f,
                          .width = static_cast<float>(text_desc.width),
                          .height = static_cast<float>(text_desc.height),
                          .min_depth = 0.0f,
                          .max_depth = 1.0f};
        RendererSystem::Get().GetImmediateContext()->SetViewports(1, &viewport);
        RendererSystem::Get().GetImmediateContext()->SetRenderTargets(1, RTVs, std::move(DSV));

    }

    RendererSystem::Get().GetImmediateContext()->CommitShaderResources(m_SRB);
    PPGE::RendererSystem::Get().GetImmediateContext()->Draw(3);
}
} // namespace PPGE
