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
    CreateDXGISwapChain();
    CreateRTVandDSV();
}

SwapChainD3D11Impl::~SwapChainD3D11Impl()
{
}

void SwapChainD3D11Impl::Present(uint32_t sync_interval)
{
    m_swap_chain_ptr->Present(sync_interval, 0);
}

std::shared_ptr<PPGETextureView> SwapChainD3D11Impl::GetBackBufferRTV()
{
    return std::static_pointer_cast<PPGETextureView>(m_rtv_sp);
}

std::shared_ptr<PPGETextureView> SwapChainD3D11Impl::GetDepthBufferDSV()
{
    return std::static_pointer_cast<PPGETextureView>(m_rtv_sp);
}

CComPtr<ID3D11RenderTargetView> SwapChainD3D11Impl::GetD3D11RenderTargetView() const
{
    CComPtr<ID3D11View> d3d11_view = m_rtv_sp->GetD3D11View();
    if (!d3d11_view)
        return nullptr;
    CComPtr<ID3D11RenderTargetView> d3d11_rtv;
    PPGE_HR(d3d11_view.QueryInterface<ID3D11RenderTargetView>(&d3d11_rtv));
    if (!d3d11_rtv)
        return nullptr;
    return d3d11_rtv;
}

CComPtr<ID3D11DepthStencilView> SwapChainD3D11Impl::GetD3D11DepthStencilView() const
{
    CComPtr<ID3D11View> d3d11_view = m_dsv_sp->GetD3D11View();
    if (!d3d11_view)
        return nullptr;
    CComPtr<ID3D11DepthStencilView> d3d11_dsv;
    PPGE_HR(d3d11_view.QueryInterface<ID3D11DepthStencilView>(&d3d11_dsv));
    if (!d3d11_dsv)
        return nullptr;
    return d3d11_dsv;
}

void SwapChainD3D11Impl::UpdateSwapChain(bool recreate)
{
    if (!m_swap_chain_ptr)
        return;

    auto d3d11_device_context_impl = m_immediate_context_wp.lock();
    if (!d3d11_device_context_impl)
        return;

    auto d3d11_device_context = d3d11_device_context_impl->GetD3D11DeviceContext();

    m_rtv_sp.reset();
    m_dsv_sp.reset();

    auto d3d11_device = m_device_wp.lock()->GetD3D11Device();
    ID3D11Debug *pDebug;
    d3d11_device->QueryInterface(IID_ID3D11Debug, (VOID **)(&pDebug));
    if (pDebug)
    {
        pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
        pDebug->Release();
    }

    if (recreate)
    {
        m_swap_chain_ptr.Release();
        d3d11_device_context->Flush();

        CreateDXGISwapChain();
    }
    else
    {
        DXGI_SWAP_CHAIN_DESC sc_desc{};
        m_swap_chain_ptr->GetDesc(&sc_desc);
        PPGE_HR(m_swap_chain_ptr->ResizeBuffers(sc_desc.BufferCount, m_desc.width, m_desc.height,
                                                sc_desc.BufferDesc.Format, sc_desc.Flags));
        d3d11_device_context->Flush();
    }

    CreateRTVandDSV();
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
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
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
    PPGE_ASSERT(d3d11_device_impl, "Creating RTV and DSV have failed: Cannot get device reference.");
    auto d3d11_device = d3d11_device_impl->GetD3D11Device();

    CComPtr<ID3D11Texture2D> d3d11_back_buffer;
    PPGE_HR(m_swap_chain_ptr->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&d3d11_back_buffer)));

    std::shared_ptr<PPGETexture> back_buffer_sp;
    d3d11_device_impl->CreateTextureFromD3D11Resource(d3d11_back_buffer, back_buffer_sp);

    TextureViewDesc rtv_desc;
    rtv_desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_RENDER_TARGET;
    rtv_desc.resource_dimension = back_buffer_sp->GetDesc().resource_dimension;
    rtv_desc.format = m_desc.color_buffer_format;
    rtv_desc.mip_levels_num = 1;
    rtv_desc.most_detailed_mip = 0;
    rtv_desc.array_slices_num = back_buffer_sp->GetDesc().array_size;
    rtv_desc.first_array_slice = 0;

    std::shared_ptr<PPGETextureView> rtv_sp = back_buffer_sp->CreateView(rtv_desc);
    m_rtv_sp = std::static_pointer_cast<PPGETextureViewD3D11>(std::move(rtv_sp));

    if (m_desc.depth_buffer_format != TextureFormatType::TEXTURE_FORMAT_UNKNOWN)
    {
        TextureCreateDesc ds_create_desc;
        ds_create_desc.desc.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D;
        ds_create_desc.desc.width = m_desc.width;
        ds_create_desc.desc.height = m_desc.height;
        ds_create_desc.desc.array_size = 1;
        ds_create_desc.desc.mip_levels = 1;
        ds_create_desc.desc.sample_count = 1;
        ds_create_desc.desc.format_type = m_desc.depth_buffer_format;
        ds_create_desc.desc.usage = UsageType::USAGE_DEFAULT;
        ds_create_desc.desc.bind_flags = BindFlags::BIND_DEPTH_STENCIL;
        ds_create_desc.desc.cpu_access_flags = CPUAccessFlags::CPU_ACCESS_NONE;
        ds_create_desc.desc.misc_flags = 0;

        std::shared_ptr<PPGETexture> ds_buffer;
        d3d11_device_impl->CreateTexture(ds_create_desc, ds_buffer);

        TextureViewDesc dsv_desc;
        dsv_desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_DEPTH_STENCIL;
        dsv_desc.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D;
        dsv_desc.format = m_desc.depth_buffer_format;
        dsv_desc.mip_levels_num = 1;
        dsv_desc.most_detailed_mip = 0;
        dsv_desc.array_slices_num = 0;
        dsv_desc.first_array_slice = 0;

        std::shared_ptr<PPGETextureView> dsv_sp = ds_buffer->CreateView(dsv_desc);
        m_dsv_sp = std::static_pointer_cast<PPGETextureViewD3D11>(std::move(dsv_sp));
    }
}

} // namespace PPGE
