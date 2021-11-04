#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/swap_chain.h"
#include "system/logger_system.h"

namespace PPGE
{
class PPGE_API SwapChainDX11 : public SwapChain
{
  public:
    SwapChainDX11(ID3D11Device *d3d11_device, bool enable_msaa);
    ~SwapChainDX11()
    {
        PPGE_RELEASE_COM(m_d3d11_swap_chain);
    }

    void Swap() override;

    IDXGISwapChain *GetD3D11SwapChainPtr()
    {
        return m_d3d11_swap_chain;
    }

    inline UINT Get4xMsaaQuality()
    {
        return m_4x_msaa_quality;
    }

  private:
    IDXGISwapChain *m_d3d11_swap_chain;
    UINT m_4x_msaa_quality;
};
} // namespace PPGE