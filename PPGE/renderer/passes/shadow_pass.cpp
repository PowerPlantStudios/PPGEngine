#include "shadow_pass.h"

#include "renderer/render_pass_resource_descs.h"

namespace PPGE
{
const std::vector<RenderPassResource> &ShadowPass::GetPassInputs() const
{
    return {};
}

const std::vector<RenderPassResource> &ShadowPass::GetPassOutputs() const
{
    return {};
}

ShadowPass::ShadowPass()
{
}

void ShadowPass::Load(RenderGraph &render_graph)
{
}

void ShadowPass::Unload()
{
}

void ShadowPass::Execute()
{
}
} // namespace PPGE
