#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/buffer.h"
#include "rhi/device_object_base.h"

namespace PPGE
{
template <typename RendererTraits>
class PPGE_API BufferBase
    : public DeviceObjectBase<typename RendererTraits::DeviceImpl, BufferDesc, typename RendererTraits::IBuffer>
{
  public:
    using BufferInterface = typename RendererTraits::IBuffer;

    using DeviceImplType = typename RendererTraits::DeviceImpl;

    using DeviceObjectBaseType = DeviceObjectBase<DeviceImplType, BufferDesc, BufferInterface>;

    BufferBase(std::shared_ptr<DeviceImplType> device_sp, const BufferDesc &desc)
        : DeviceObjectBaseType(std::move(device_sp), desc)
    {
    }
};
} // namespace PPGE