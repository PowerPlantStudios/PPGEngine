#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/render_pass.h"
#include "renderer/render_pass_resource_descs.h"
#include "rhi/pipeline_state.h"
#include "rhi/sampler.h"
#include "rhi/shader_resource_binding.h"

namespace PPGE
{
class PresentPass : public RenderPass
{
  public:
    PresentPass();

    std::vector<RenderPassResource> GetPassInputs() const override final
    {
        return {
            {RenderPassResourceDescs::Present_Buffer_Resource, RenderPassResourceDescs::Present_Buffer_Resource_Desc}};
    }

    void Load(RenderGraph &render_graph) override final;

    void Unload() override final;

    void Execute() override final;

  private:
    /* Resources created and used within the pass */
    std::shared_ptr<PPGESampler> m_sampler_state;

    std::shared_ptr<PPGEPipelineState> m_PSO;

    std::shared_ptr<PPGEShaderResourceBinding> m_SRB;
};
} // namespace PPGE
