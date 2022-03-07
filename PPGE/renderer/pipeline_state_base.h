#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"
#include "renderer/device_object_base.h"
#include "renderer/pipeline_state.h"

namespace PPGE
{
template <typename RendererTraits>
class PPGE_API PipelineStateBase : public DeviceObjectBase<typename RendererTraits::DeviceImpl, PipelineStateDesc,
                                                           typename RendererTraits::IPipelineState>,
                                   public std::enable_shared_from_this<typename RendererTraits::PipelineStateImpl>
{
  public:
    using PipelineStateInterface = typename RendererTraits::IPipelineState;

    using DeviceImplType = typename RendererTraits::DeviceImpl;

    using DeviceObjectBaseType = DeviceObjectBase<DeviceImplType, PipelineStateDesc, PipelineStateInterface>;

    PipelineStateBase(std::shared_ptr<DeviceImplType> device_sp, const GfxPipelineStateCreateDesc &desc)
        : DeviceObjectBaseType(std::move(device_sp), desc.desc), m_pso_type(desc.desc.type)
    {
        if (m_pso_type == PipelineType::PIPELINE_GRAPHICS)
            gfx_pso_desc_up = std::make_unique<GfxPipelineStateDesc>(desc.desc);
    }

    const GfxPipelineStateDesc &GetGraphicsPipelineStateDesc() const override final
    {
        PPGE_ASSERT(m_pso_type == PipelineType::PIPELINE_GRAPHICS,
                    "Gfx pipeline state description is requested from a non gfx pipeline state object.");
        return *gfx_pso_desc_up;
    }

    PipelineType GetPipelineType() const override final
    {
        return m_pso_type;
    }

  protected:
    PipelineType m_pso_type;

    std::unique_ptr<GfxPipelineStateDesc> gfx_pso_desc_up;
};
} // namespace PPGE
