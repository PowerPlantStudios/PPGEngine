#include "platform/dx11/swap_chain_dx11.h"

#include "system/display_system.h"

namespace PPGE
{
SwapChainDX11::SwapChainDX11(ID3D11Device *d3d11_device, bool enable_msaa)
{
    m_d3d11_swap_chain = NULL;
    m_4x_msaa_quality = 0;
    PPGE_ASSERT(d3d11_device, "D3D11 device is null");

    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = DisplaySystem::Get().GetWidth();
    sd.BufferDesc.Height = DisplaySystem::Get().GetHeight();
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    if (enable_msaa)
    {
        PPGE_HR(d3d11_device->CheckMultisampleQualityLevels(
            DXGI_FORMAT_R8G8B8A8_UNORM, // Render target format to check quality level for the given sample count
            4,                          // Sample count for MSAA
            &m_4x_msaa_quality // [out] Supported max quality level for the given render target format and sample count
            ));
        // Sample count 4 should be supported for all render targets for DX11 capable devices
        PPGE_ASSERT((m_4x_msaa_quality > 0), "For the given render target and sample count, MSAA is not supported.");

        sd.SampleDesc.Count = 4;
        sd.SampleDesc.Quality = m_4x_msaa_quality - 1;
    }
    else
    {
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
    }

    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = static_cast<HWND>(DisplaySystem::Get().GetNativeDisplayPtr());
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    IDXGIDevice *dxgi_device = NULL;
    PPGE_HR(d3d11_device->QueryInterface(__uuidof(IDXGIDevice), (void **)&dxgi_device));
    IDXGIAdapter *dxgi_adapter = NULL;
    PPGE_HR(dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void **)&dxgi_adapter));
    IDXGIFactory *dxgi_factory = NULL;
    PPGE_HR(dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void **)&dxgi_factory));

    PPGE_HR(dxgi_factory->CreateSwapChain(d3d11_device, &sd, &m_d3d11_swap_chain));

    PPGE_RELEASE_COM(dxgi_device);
    PPGE_RELEASE_COM(dxgi_adapter);
    PPGE_RELEASE_COM(dxgi_factory);
}

void SwapChainDX11::Swap()
{
    m_d3d11_swap_chain->Present(0, 0);
}
} // namespace PPGE