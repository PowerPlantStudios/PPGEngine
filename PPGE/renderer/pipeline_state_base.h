#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/device_object_base.h"
#include "renderer/pipeline_state.h"
#include "system/logger_system.h"

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
        : DeviceObjectBaseType(std::move(device_sp), desc.desc)
    {
        m_gfx_ps_desc = desc.desc;
    }

    PipelineStateBase(std::shared_ptr<DeviceImplType> device_sp, const RtxPipelineStateCreateDesc &desc)
        : DeviceObjectBaseType(std::move(device_sp), desc.desc)
    {
        m_rtx_ps_desc = desc.desc;
    }

    const GfxPipelineStateDesc &GetGraphicsPipelineStateDesc() const override final
    {
        PPGE_ASSERT(DeviceObjectBaseType::m_desc.type == PipelineType::PIPELINE_GRAPHICS,
                    "Gfx pipeline state description is requested from a non gfx pipeline state object.");
        return m_gfx_ps_desc;
    }

    const RtxPipelineStateDesc &GetRayTracingPipelineStateDesc() const override final
    {
        PPGE_ASSERT(DeviceObjectBaseType::m_desc.type == PipelineType::PIPELINE_RAYTRACING,
                    "Gfx pipeline state description is requested from a non gfx pipeline state object.");
        return m_rtx_ps_desc;
    }

    PipelineType GetPipelineType() const override final
    {
        return DeviceObjectBaseType::m_desc.type;
    }

    uint16_t GetStride(uint16_t buffer_slot) const
    {
        PPGE_ASSERT(buffer_slot < Num_Of_Buffer_Slots, "Buffer slot cannot exceeds the limit.")
        return DeviceObjectBaseType::m_desc.type == PipelineType::PIPELINE_GRAPHICS ? m_layout_strides[buffer_slot] : 0;
    }

  protected:
    GfxPipelineStateDesc m_gfx_ps_desc;

    RtxPipelineStateDesc m_rtx_ps_desc;

    std::array<uint16_t, Num_Of_Buffer_Slots> m_layout_strides;
};
} // namespace PPGE
