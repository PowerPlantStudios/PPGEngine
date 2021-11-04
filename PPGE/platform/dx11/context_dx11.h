#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/context.h"
#include "system/logger_system.h"

namespace PPGE
{
class PPGE_API ContextDX11 : public Context
{
  public:
    ContextDX11(ID3D11Device *d3d11_device)
    {
        m_d3d11_immediate_context = NULL;
        PPGE_ASSERT(d3d11_device, "D3D11 device is null");
        d3d11_device->GetImmediateContext(&m_d3d11_immediate_context);
    }
    ~ContextDX11()
    {
        PPGE_RELEASE_COM(m_d3d11_immediate_context);
    }

    ID3D11DeviceContext *GetD3D11DeviceContextPtr()
    {
        return m_d3d11_immediate_context;
    }

  private:
    ID3D11DeviceContext *m_d3d11_immediate_context;
};
} // namespace PPGE