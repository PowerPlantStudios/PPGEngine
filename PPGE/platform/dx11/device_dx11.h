#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/device.h"
#include "system/logger_system.h"

namespace PPGE
{
class PPGE_API DeviceDX11 : public Device
{
  public:
    DeviceDX11();
    ~DeviceDX11()
    {
        PPGE_RELEASE_COM(m_d3d11_immediate_context);
        PPGE_RELEASE_COM(m_d3d11_device);
    }

    ID3D11Device *GetD3D11DevicePtr()
    {
        return m_d3d11_device;
    }

  private:
    ID3D11Device *m_d3d11_device;
    ID3D11DeviceContext *m_d3d11_immediate_context;
};
} // namespace PPGE