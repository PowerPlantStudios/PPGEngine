#include "swap_chain_d3d11_impl.h"

#include "platform/d3d11/device_context_d3d11_impl.h"
#include "platform/d3d11/device_d3d11_impl.h"
#include "platform/d3d11/renderer_types_d3d11_conversion.h"
#include "system/display_system.h"

namespace PPGE
{
SwapChainD3D11Impl::SwapChainD3D11Impl(std::weak_ptr<DeviceD3D11Impl> device_wp,
                                       std::weak_ptr<DeviceContextD3D11Impl> immediate_context_wp,
                                       const SwapChainDesc &desc)
    : SwapChainBaseType(std::move(device_wp), std::move(immediate_context_wp), desc)
{
}

SwapChainD3D11Impl::~SwapChainD3D11Impl()
{
}

void SwapChainD3D11Impl::CreateDXGISwapChain()
{
    auto d3d11_device_impl = m_device_wp.lock();
    PPGE_ASSERT(d3d11_device_impl, "Creating swap chain has failed: Cannot get device reference.");
    auto d3d11_device = d3d11_device_impl->GetD3D11Device();

    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    swap_chain_desc.BufferDesc.Width = m_desc.width;
    swap_chain_desc.BufferDesc.Height = m_desc.height;
    swap_chain_desc.BufferDesc.RefreshRate.Numerator = m_desc.refresh_rate_num;
    swap_chain_desc.BufferDesc.RefreshRate.Denominator = m_desc.refresh_rate_denum;
    swap_chain_desc.BufferDesc.Format = PPGETextureFormatToD3D11TextureFormat(m_desc.color_buffer_format);
    swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.SampleDesc.Quality = 0;

    swap_chain_desc.BufferUsage = PPGESwapChainUsageFlagsToD3D11SwapChainUsageFlags(m_desc.swap_chain_usages);

    swap_chain_desc.BufferCount = m_desc.buffer_count;

    swap_chain_desc.OutputWindow = static_cast<HWND>(DisplaySystem::Get().GetNativeDisplayPtr());
    swap_chain_desc.Windowed = true;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swap_chain_desc.Flags = 0;

    CComPtr<IDXGIDevice> dxgi_device = NULL;
    PPGE_HR(d3d11_device->QueryInterface(__uuidof(IDXGIDevice), (void **)&dxgi_device));
    CComPtr<IDXGIAdapter> dxgi_adapter = NULL;
    PPGE_HR(dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void **)&dxgi_adapter));
    CComPtr<IDXGIFactory> dxgi_factory = NULL;
    PPGE_HR(dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void **)&dxgi_factory));

    PPGE_HR(dxgi_factory->CreateSwapChain(d3d11_device, &swap_chain_desc, &m_swap_chain_ptr));
}

void SwapChainD3D11Impl::CreateRTVandDSV()
{
    auto d3d11_device_impl = m_device_wp.lock();
    auto d3d11_device_context_impl = m_immediate_context_wp.lock();

    if (!d3d11_device_impl || !d3d11_device_context_impl)
        return;

    auto d3d11_device = d3d11_device_impl->GetD3D11Device();
    auto d3d11_device_context = d3d11_device_context_impl->GetD3D11DeviceContext();

    //m_RTV.Release();
    //m_DSV.Release();
    //m_DS_buffer.Release();

    //PPGE_HR(m_swap_chain_ptr->ResizeBuffers(1, DisplaySystem::Get().GetWidth(), DisplaySystem::Get().GetHeight(),
    //                                    DXGI_FORMAT_R8G8B8A8_UNORM, 0));
    
    CComPtr<ID3D11Texture2D> d3d11_back_buffer;
    PPGE_HR(m_swap_chain_ptr->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&d3d11_back_buffer)));

    std::shared_ptr<PPGETexture> back_buffer_sp;
    std::shared_ptr<PPGETextureView> rtv_sp;
    TextureViewDesc rtv_desc;
    rtv_desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_RENDER_TARGET;
    rtv_desc.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D;
    rtv_desc.format = m_desc.color_buffer_format;

    /*
            ResourceViewType texture_view_type = ResourceViewType::RESOURCE_VIEW_UNDEFINED;
            ResourceDimensionType resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_UNDEFINED;
            TextureFormatType format = TextureFormatType::TEXTURE_FORMAT_UNKNOWN;
            uint32_t most_detailed_mip = 0;
            uint32_t mip_levels_num = 0;
            union {
                uint32_t first_array_slice = 0;
                uint32_t first_depth_slice;
            };
            union {
                uint32_t array_slices_num = 0;
                uint32_t depth_slices_num;
            };
    */

    d3d11_device_impl->CreateTextureFromD3D11Resource(d3d11_back_buffer.p, back_buffer_sp);
    back_buffer_sp->CreateView(rtv_desc, rtv_sp);

    //PPGE_HR(d3d11_device->CreateRenderTargetView(back_buffer, 0, &m_RTV));

    //D3D11_TEXTURE2D_DESC depthStencilDesc;
    //depthStencilDesc.Width = DisplaySystem::Get().GetWidth();
    //depthStencilDesc.Height = DisplaySystem::Get().GetHeight();
    //depthStencilDesc.MipLevels = 1;
    //depthStencilDesc.ArraySize = 1;
    //depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    //depthStencilDesc.SampleDesc.Count = 1;
    //depthStencilDesc.SampleDesc.Quality = 0;

    //depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    //depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    //depthStencilDesc.CPUAccessFlags = 0;
    //depthStencilDesc.MiscFlags = 0;

    //PPGE_HR(d3d11_device->CreateTexture2D(&depthStencilDesc, 0, &m_DS_buffer));
    //PPGE_HR(d3d11_device->CreateDepthStencilView(m_DS_buffer, 0, &m_DSV));

    //d3d11_immediate_context->OMSetRenderTargets(1, &m_RTV.p, m_DSV);

    //m_viewport.TopLeftX = 0;
    //m_viewport.TopLeftY = 0;
    //m_viewport.Width = DisplaySystem::Get().GetWidth();
    //m_viewport.Height = DisplaySystem::Get().GetHeight();
    //m_viewport.MinDepth = 0.0f;
    //m_viewport.MaxDepth = 1.0f;

    //d3d11_immediate_context->RSSetViewports(1, &m_viewport);
}

} // namespace PPGE
