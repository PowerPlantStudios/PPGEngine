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

    const std::vector<RenderPassResource> &GetPassInputs() const override final
    {
        return m_pass_inputs;
    }

    const std::vector<RenderPassResource> &GetPassOutputs() const override final
    {
        return m_pass_outputs;
    }

    void Load(RenderGraph &render_graph) override final;

    void Unload() override final;

    void Execute() override final;

  private:
    /* Constant pass Input/Output declerations */
    const std::vector<RenderPassResource> m_pass_inputs{
        {RenderPassResourceDescs::Shadow_Map_Resource, RenderPassResourceDescs::Shadow_Map_Resource_Desc},
        {RenderPassResourceDescs::Color_Buffer_Resource, RenderPassResourceDescs::Color_Buffer_Resource_Desc},
        {RenderPassResourceDescs::Depth_Buffer_Resource, RenderPassResourceDescs::Depth_Buffer_Resource_Desc}};

    const std::vector<RenderPassResource> m_pass_outputs{
        {RenderPassResourceDescs::Color_Buffer_Resource, RenderPassResourceDescs::Color_Buffer_Resource_Desc},
        {RenderPassResourceDescs::Depth_Buffer_Resource, RenderPassResourceDescs::Depth_Buffer_Resource_Desc}};

    /* Resources shared between other render passes within render graph */
    std::shared_ptr<PPGETextureView> m_color_buffer_rtv;

    std::shared_ptr<PPGETextureView> m_depth_buffer_dsv;

    std::shared_ptr<PPGETextureView> m_shadow_buffer_srv;

    /* Resources created and used within the pass */
    std::shared_ptr<PPGETextureView> m_unknown_texture;

    std::shared_ptr<PPGEBuffer> m_cb_per_object_vs;

    std::shared_ptr<PPGEBuffer> m_cb_per_frame_ps;

    std::shared_ptr<PPGEBuffer> m_cb_per_object_ps;

    std::shared_ptr<PPGESampler> m_sampler_state;

    std::shared_ptr<PPGEPipelineState> m_PSO;

    std::shared_ptr<PPGEShaderResourceBinding> m_SRB;
};
} // namespace PPGE