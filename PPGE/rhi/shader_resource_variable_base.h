#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/device_object_base.h"
#include "rhi/pipeline_state.h"

namespace PPGE
{
template <typename ShaderResourceVariableInterface, typename ShaderResourceVariableImplType,
          typename DeviceResourceImplType>
class PPGE_API ShaderResourceVariableBase : public ShaderResourceVariableInterface
{
  public:
    ShaderResourceVariableBase(const ShaderResourceDesc &desc) : m_desc(desc)
    {
    }

    const ShaderResourceDesc &GetDesc() const override final
    {
        return m_desc;
    }

    void Set(std::shared_ptr<PPGEDeviceObject> device_object_sp) override final
    {
        static_cast<ShaderResourceVariableImplType *>(this)->BindResource(std::move(device_object_sp));
    }

    std::shared_ptr<PPGEDeviceObject> Get() const override final
    {
        return static_cast<const ShaderResourceVariableImplType *>(this)->GetResource();
    }

  protected:
    virtual void BindResource(std::shared_ptr<PPGEDeviceObject> device_object_sp) = 0;

    virtual std::shared_ptr<PPGEDeviceObject> GetResource() const = 0;

    ShaderResourceDesc m_desc;

    std::shared_ptr<DeviceResourceImplType> m_device_resource_sp;
};
} // namespace PPGE