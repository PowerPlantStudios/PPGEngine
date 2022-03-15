#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "platform/d3d11/texture_view_d3d11.h"
#include "renderer/swap_chain.h"

namespace PPGE
{
class PPGE_API PPGESwapChainD3D11 : public PPGESwapChain
{
  public:
    virtual CComPtr<IDXGISwapChain> GetDXGISwapChain() const = 0;

    virtual CComPtr<ID3D11RenderTargetView> GetD3D11RenderTargetView() const = 0;

    virtual CComPtr<ID3D11DepthStencilView> GetD3D11DepthStencilView() const = 0;
};
} // namespace PPGE
