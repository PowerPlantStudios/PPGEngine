#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/render_pass.h"

namespace PPGE
{
class PPGEBuffer;
class PPGEPipelineState;
class PPGEShaderResourceBinding;

class ShadowPass : public SceneRenderPass
{
  public:
    ShadowPass();

    void Load(RenderGraph &render_graph) override final;

    void Unload() override final;

    void Execute() override final;

  private:
    /* Resources created and used within the pass */
    std::shared_ptr<PPGEBuffer> m_cb_per_draw;

    std::shared_ptr<PPGEBuffer> m_cb_per_frame;

    std::shared_ptr<PPGEPipelineState> m_PSO;

    std::shared_ptr<PPGEShaderResourceBinding> m_SRB;
};
} // namespace PPGE
