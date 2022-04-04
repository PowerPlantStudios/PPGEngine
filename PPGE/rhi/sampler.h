#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/device_object.h"
#include "rhi/renderer_types.h"

namespace PPGE
{
struct SamplerDesc : public DeviceObjectDesc
{
};

class PPGE_API PPGESampler : public PPGEDeviceObject
{
  public:
    virtual const SamplerDesc &GetDesc() const = 0;
};
} // namespace PPGE
