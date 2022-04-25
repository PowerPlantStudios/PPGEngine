#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/render_pass.h"

namespace PPGE
{
class ShadowPass : public RenderPass
{
  public:
    const std::vector<RenderPassResource> &GetPassInputs() const override final;

    const std::vector<RenderPassResource> &GetPassOutputs() const override final;

    ShadowPass();

    void Load(RenderGraph &render_graph) override final;

    void Unload() override final;

    void Execute() override final;
};
} // namespace PPGE
