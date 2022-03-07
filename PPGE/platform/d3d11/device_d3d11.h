#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/device.h"

namespace PPGE
{
class PPGE_API PPGEDeviceD3D11 : public PPGEDevice
{
  public:
    virtual ID3D11Device *GetD3D11Device() const = 0;
};
} // namespace PPGE