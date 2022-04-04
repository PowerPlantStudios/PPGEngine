#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/ppge_object.h"

namespace PPGE
{
struct DeviceObjectDesc
{
    const char *name = nullptr;
};

class PPGEDeviceObject : public PPGEObject
{
  public:
    virtual void SetUserData(PPGEObject *p_data) = 0;
    virtual PPGEObject *GetUserData() const = 0;
};
} // namespace PPGE
