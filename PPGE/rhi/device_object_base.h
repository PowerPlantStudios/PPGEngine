#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"
#include "core/ppge_object.h"

namespace PPGE
{
template <typename DeviceImplType, typename DeviceObjectDescType, typename BaseInterface>
class PPGE_API DeviceObjectBase : public BaseInterface
{
  public:
    DeviceObjectBase(std::shared_ptr<DeviceImplType> device_sp, const DeviceObjectDescType &desc)
        : m_device_sp(std::move(device_sp)), m_desc(desc), m_user_ptr(nullptr)
    {
    }

    virtual ~DeviceObjectBase()
    {
    }

    void SetUserData(PPGEObject *p_data) override final
    {
        m_user_ptr = p_data;
    }

    PPGEObject *GetUserData() const override final
    {
        return m_user_ptr;
    }

    const DeviceObjectDescType &GetDesc() const override final
    {
        return m_desc;
    }

    inline bool HasDevice() const
    {
        return m_device_sp;
    }

    DeviceImplType *GetDevice() const
    {
        PPGE_ASSERT(m_device_sp, "PPGE device is lost.");
        return m_device_sp;
    }

  protected:
    std::shared_ptr<DeviceImplType> m_device_sp;

    DeviceObjectDescType m_desc;
    
    PPGEObject *m_user_ptr;
};
} // namespace PPGE
