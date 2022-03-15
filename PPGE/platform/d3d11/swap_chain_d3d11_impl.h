#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "platform/d3d11/renderer_traits_d3d11.h"
#include "platform/d3d11/swap_chain_d3d11.h"
#include "renderer/swap_chain_base.h"

namespace PPGE
{
class SwapChainD3D11Impl final : public SwapChainBase<RendererTraitsD3D11, PPGESwapChainD3D11>
{
  public:
    using SwapChainBaseType = SwapChainBase<RendererTraitsD3D11, PPGESwapChainD3D11>;

    SwapChainD3D11Impl(std::weak_ptr<DeviceD3D11Impl> device_wp,
                       std::weak_ptr<DeviceContextD3D11Impl> immediate_context_wp, const SwapChainDesc &desc);

    ~SwapChainD3D11Impl();

    void Present(uint32_t sync_interval) override final;

    std::shared_ptr<PPGETextureView> GetBackBufferRTV() override final;

    std::shared_ptr<PPGETextureView> GetDepthBufferDSV() override final;

    CComPtr<IDXGISwapChain> GetDXGISwapChain() const override final
    {
        return m_swap_chain_ptr;
    }

    CComPtr<ID3D11RenderTargetView> GetD3D11RenderTargetView() const override final;

    CComPtr<ID3D11DepthStencilView> GetD3D11DepthStencilView() const override final;

  private:
    void UpdateSwapChain(bool recreate) override final;

    void CreateDXGISwapChain();

    void CreateRTVandDSV();

    CComPtr<IDXGISwapChain> m_swap_chain_ptr;

    std::shared_ptr<PPGETextureViewD3D11> m_rtv_sp;
    std::shared_ptr<PPGETextureViewD3D11> m_dsv_sp;
};
} // namespace PPGE