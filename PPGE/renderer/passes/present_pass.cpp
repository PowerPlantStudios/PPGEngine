#include "present_pass.h"

#include "renderer/render_pass_resource_descs.h"

namespace PPGE
{
const std::vector<RenderPassResource> &PresentPass::GetPassInputs() const
{
    return m_pass_inputs;
}

const std::vector<RenderPassResource> &PresentPass::GetPassOutputs() const
{
    return m_pass_outputs;
}

PresentPass::PresentPass()
{
}

void PresentPass::Load(RenderGraph &render_graph)
{
}

void PresentPass::Unload()
{
}

void PresentPass::Execute()
{
}
} // namespace PPGE
