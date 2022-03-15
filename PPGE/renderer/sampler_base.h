#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/device_object_base.h"
#include "renderer/sampler.h"
#include "system/logger_system.h"

namespace PPGE
{
template <typename RendererTraits>
class PPGE_API SamplerBase
    : public DeviceObjectBase<typename RendererTraits::DeviceImpl, SamplerDesc, typename RendererTraits::ISampler>
{
  public:
    using SamplerInterface = typename RendererTraits::ISampler;

    using DeviceImplType = typename RendererTraits::DeviceImpl;

    using DeviceObjectBaseType = DeviceObjectBase<DeviceImplType, SamplerDesc, SamplerInterface>;

    SamplerBase(std::shared_ptr<DeviceImplType> device_sp, const SamplerDesc &desc)
        : DeviceObjectBaseType(std::move(device_sp), desc)
    {
    }
};
} // namespace PPGE