#include "platform/d3d11/renderer_system_d3d11.h"

#include "platform/d3d11/device_context_d3d11_impl.h"
#include "platform/d3d11/device_d3d11_impl.h"
#include "platform/d3d11/imgui_widget_d3d11.h"
#include "platform/d3d11/swap_chain_d3d11_impl.h"
#include "system/display_system.h"

namespace PPGE
{
static UINT GetMSAASampleCount(MSAAQuality quality)
{
    switch (quality)
    {
    case PPGE::MSAAQuality::MSAA_OFF:
        return 0;
    case PPGE::MSAAQuality::MSAA_2X:
        return 2;
    case PPGE::MSAAQuality::MSAA_4X:
        return 4;
    case PPGE::MSAAQuality::MSAA_8X:
        return 8;
    case PPGE::MSAAQuality::MSAA_16X:
        return 16;
    default:
        PPGE_ASSERT(false, "Unknown MSAA Quality");
    }
    return 0;
}

void RendererSystemD3D11::StartUp(const RendererSystemProps &props)
{
    CComPtr<ID3D11Device> d3d11_device_ptr;
    CComPtr<ID3D11DeviceContext> d3d11_device_context_ptr;

    UINT device_flags = 0;
#if defined(PPGE_DEBUG)
    device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    const D3D_FEATURE_LEVEL target_feature_levels[] = {D3D_FEATURE_LEVEL_11_1};
    D3D_FEATURE_LEVEL feature_level;
    // clang-fomat off
    PPGE_HR(D3D11CreateDevice(
        0,                               // Chosen adapter. '0' means default adapter.
        D3D_DRIVER_TYPE_HARDWARE,        // Driver type. Prefer HW Driver for performance.
        0,                               // If SW Driver is specified, handle to SW driver should be provided
        device_flags,                    // Device flags for configuration.
        target_feature_levels,           // Ptr to feature level array for D3D11 to be created with.
        _countof(target_feature_levels), // # of elements in the feature level array.
        D3D11_SDK_VERSION, // Set SDK version to be used. Set this to D3D11_SDK_Version to support Windows Store apps.
        &d3d11_device_ptr, // [out] Created d3d11 device.
        &feature_level,    // [out] Feature level for created device.
        &d3d11_device_context_ptr // [out] Created d3d11 immediate context.
        ));
    // clang-format on
#if defined(PPGE_DEBUG)
    if (feature_level != D3D_FEATURE_LEVEL_11_1)
        PPGE_ERROR("PPGE requires Direct3D feature level 11.1.");
#endif
    CComQIPtr<ID3D11Device1> d3d11_device1_ptr{d3d11_device_ptr};
    CComQIPtr<ID3D11DeviceContext1> d3d11_device_context1_ptr{d3d11_device_context_ptr};
    PPGE_ASSERT(d3d11_device1_ptr && d3d11_device_context1_ptr,
                "Failed to create device and device context with newer interface.");

    m_device_sp = std::make_shared<DeviceD3D11Impl>(d3d11_device1_ptr);
    m_device_context_sp =
        std::make_shared<DeviceContextD3D11Impl>(m_device_sp, d3d11_device_context1_ptr, DeviceContextDesc{});
    m_device_sp->SetDeviceContext(m_device_context_sp);

    SwapChainDesc sc_desc;
    sc_desc.width = DisplaySystem::Get().GetWidth();
    sc_desc.height = DisplaySystem::Get().GetHeight();
    sc_desc.refresh_rate_num = 60;
    sc_desc.refresh_rate_denum = 1;
    sc_desc.color_buffer_format = TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_UNORM;
    sc_desc.depth_buffer_format = TextureFormatType::TEXTURE_FORMAT_D24_UNORM_S8_UINT;
    sc_desc.swap_chain_usages = SwapChainUsageFlags::SWAP_CHAIN_USAGE_RENDER_TARGET_OUTPUT;
    sc_desc.buffer_count = 2;
    m_swap_chain_sp = std::make_shared<SwapChainD3D11Impl>(m_device_sp, m_device_context_sp, sc_desc);

    OnResize();
}

void RendererSystemD3D11::Update()
{
    m_swap_chain_sp->Present(1);

    auto d3d11_immediate_context = m_device_context_sp->GetD3D11DeviceContext();
    auto d3d11_rtv = m_swap_chain_sp->GetD3D11RenderTargetView();
    auto d3d11_dsv = m_swap_chain_sp->GetD3D11DepthStencilView();

    float clear_color[] = {0.8, 0.8, 0.8, 1.0f};
    d3d11_immediate_context->ClearRenderTargetView(d3d11_rtv, clear_color);

    UINT clear_flag = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL;
    d3d11_immediate_context->ClearDepthStencilView(d3d11_dsv, clear_flag, 1.0f, 1);

    ID3D11RenderTargetView *d3d11_rtvs[] = {d3d11_rtv.p};

    d3d11_immediate_context->OMSetRenderTargets(1, d3d11_rtvs, d3d11_dsv);
}

void RendererSystemD3D11::ShutDown()
{
}

void RendererSystemD3D11::OnResize()
{
    if (!m_swap_chain_sp)
        return;

    m_swap_chain_sp->Resize(DisplaySystem::Get().GetWidth(), DisplaySystem::Get().GetHeight());

    auto d3d11_immediate_context = m_device_context_sp->GetD3D11DeviceContext();

    auto d3d11_rtv = m_swap_chain_sp->GetD3D11RenderTargetView();
    ID3D11RenderTargetView* d3d11_rtvs[] = {d3d11_rtv.p};

    auto d3d11_dsv = m_swap_chain_sp->GetD3D11DepthStencilView();

    d3d11_immediate_context->OMSetRenderTargets(1, d3d11_rtvs, d3d11_dsv);

    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;
    m_viewport.Width = DisplaySystem::Get().GetWidth();
    m_viewport.Height = DisplaySystem::Get().GetHeight();
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    d3d11_immediate_context->RSSetViewports(1, &m_viewport);
}

PPGEDevice *RendererSystemD3D11::GetDevice()
{
    return static_cast<PPGEDevice *>(m_device_sp.get());
}

PPGEDeviceContext *RendererSystemD3D11::GetImmediateContext()
{
    return static_cast<PPGEDeviceContexD3D11 *>(m_device_context_sp.get());
}

} // namespace PPGE