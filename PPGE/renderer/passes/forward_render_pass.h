#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/render_pass.h"
#include "renderer/render_pass_resource_descs.h"
#include "rhi/buffer.h"
#include "rhi/pipeline_state.h"
#include "rhi/rhi_types.h"
#include "rhi/sampler.h"
#include "rhi/shader_resource_binding.h"
#include "rhi/texture_view.h"
#include "system/logger_system.h"

namespace PPGE
{
class RenderGraph;

class ForwardRenderPass : public SceneRenderPass
{
  public:
    ForwardRenderPass();

    std::vector<RenderPassResource> GetPassInputs() const override final
    {
        return {{RenderPassResourceDescs::Color_Buffer_Resource, RenderPassResourceDescs::Color_Buffer_Resource_Desc},
                {RenderPassResourceDescs::Depth_Buffer_Resource, RenderPassResourceDescs::Depth_Buffer_Resource_Desc}};
    }

    std::vector<RenderPassResource> GetPassOutputs() const override final
    {
        return {{RenderPassResourceDescs::Color_Buffer_Resource, RenderPassResourceDescs::Color_Buffer_Resource_Desc},
                {RenderPassResourceDescs::Depth_Buffer_Resource, RenderPassResourceDescs::Depth_Buffer_Resource_Desc}};
    }

    void Load(RenderGraph &render_graph) override final;

    void Unload() override final;

    void Execute() override final;

  private:
    void Draw();

    /* Resources shared between other render passes within render graph */
    std::shared_ptr<PPGETextureView> m_color_buffer_rtv;

    std::shared_ptr<PPGETextureView> m_depth_buffer_dsv;

    /* Resources created and used within the pass */
    std::shared_ptr<PPGETextureView> m_unknown_texture;

    std::shared_ptr<PPGEBuffer> m_cb_per_draw;

    std::shared_ptr<PPGEBuffer> m_cb_light;

    std::shared_ptr<PPGESampler> m_comparison_sampler;

    std::shared_ptr<PPGESampler> m_anisotropic_sampler;

    std::shared_ptr<PPGEPipelineState> m_ambient_PSO;

    std::shared_ptr<PPGEPipelineState> m_lighting_PSO;

    std::shared_ptr<PPGEShaderResourceBinding> m_SRB;
};
} // namespace PPGE