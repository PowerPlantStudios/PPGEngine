#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/device_object_base.h"
#include "renderer/pipeline_state.h"

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

    PPGEDeviceObject *Get() const override final
    {
        return reinterpret_cast<PPGEDeviceObject *>(m_device_resource_sp.get());
    }

  protected:
    virtual void BindResource(std::shared_ptr<PPGEDeviceObject> device_object_sp) = 0;

    ShaderResourceDesc m_desc;

    std::shared_ptr<DeviceResourceImplType> m_device_resource_sp;
};
} // namespace PPGE