#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/render_pass.h"
#include "renderer/render_pass_resource_descs.h"

namespace PPGE
{
class PresentPass : public RenderPass
{
  public:
    const std::vector<RenderPassResource> &GetPassInputs() const override final;

    const std::vector<RenderPassResource> &GetPassOutputs() const override final;

    PresentPass();

    void Load(RenderGraph &render_graph) override final;

    void Unload() override final;

    void Execute() override final;

  private:
    const std::vector<RenderPassResource> m_pass_inputs{
        {RenderPassResourceDescs::Color_Buffer_Resource, RenderPassResourceDescs::Color_Buffer_Resource_Desc},
        {RenderPassResourceDescs::Depth_Buffer_Resource, RenderPassResourceDescs::Depth_Buffer_Resource_Desc}};

    const std::vector<RenderPassResource> m_pass_outputs;
};
} // namespace PPGE
