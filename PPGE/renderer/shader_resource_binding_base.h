#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/shader_resource_binding.h"

namespace PPGE
{
template <typename RendererTraits>
class PPGE_API ShaderResourceBindingBase : public RendererTraits::IShaderResourceBinding
{
  public:
    using ShaderResourceBindingInterface = typename RendererTraits::IShaderResourceBinding;

    using PipelineStateImplType = typename RendererTraits::PipelineStateImpl;

    ShaderResourceBindingBase(std::shared_ptr<PipelineStateImplType> PSO_sp) : m_PSO_sp(std::move(PSO_sp))
    {
    }

  protected:
    std::shared_ptr<PipelineStateImplType> m_PSO_sp;
};
} // namespace PPGE
