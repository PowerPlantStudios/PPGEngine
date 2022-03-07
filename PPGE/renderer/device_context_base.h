#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"

namespace PPGE
{
template <typename RendererTraits> class PPGE_API DeviceContextBase : public RendererTraits::IDeviceContext
{
  public:
    using DeviceContextInterface = typename RendererTraits::IDeviceContext;

    using DeviceImplType = typename RendererTraits::DeviceImpl;
    using DeviceContextImplType = typename RendererTraits::DeviceContextImpl;
    using BufferImplType = typename RendererTraits::BufferImpl;
    using ShaderImplType = typename RendererTraits::ShaderImpl;
    using PipelineStateImplType = typename RendererTraits::PipelineStateImpl;

    DeviceContextBase(std::shared_ptr<DeviceImplType> device_sp, const DeviceContextDesc &desc)
        : m_device_sp(std::move(device_sp)), m_desc(desc)
    {
    }

    ~DeviceContextBase()
    {
    }

  protected:
    std::shared_ptr<DeviceImplType> m_device_sp;

    DeviceContextDesc m_desc;
};
} // namespace PPGE