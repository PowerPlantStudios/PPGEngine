#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/device_context.h"

namespace PPGE
{
class PPGE_API PPGEDeviceContexD3D11 : public PPGEDeviceContext
{
  public:
    virtual CComPtr<ID3D11DeviceContext> GetD3D11DeviceContext() const = 0;
};
} // namespace PPGE
