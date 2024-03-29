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
}

void RendererSystemD3D11::ShutDown()
{
}

void RendererSystemD3D11::OnResize()
{
    if (!m_swap_chain_sp)
        return;

    m_device_context_sp->SetRenderTargets(0, nullptr, nullptr);

    m_swap_chain_sp->Resize(DisplaySystem::Get().GetWidth(), DisplaySystem::Get().GetHeight());

    auto color_buffer_rtv = m_swap_chain_sp->GetBackBufferRTV();
    auto depth_buffer_dsv = m_swap_chain_sp->GetDepthBufferDSV();
    m_device_context_sp->SetRenderTargets(1, &color_buffer_rtv, depth_buffer_dsv);

    Viewport viewport;
    viewport.top_left_x = 0.0f;
    viewport.top_left_y = 0.0f;
    viewport.width = DisplaySystem::Get().GetWidth();
    viewport.height = DisplaySystem::Get().GetHeight();
    viewport.min_depth = 0.0f;
    viewport.max_depth = 1.0f;
    m_device_context_sp->SetViewports(1, &viewport);
}

PPGEDevice *RendererSystemD3D11::GetDevice()
{
    return static_cast<PPGEDevice *>(m_device_sp.get());
}

PPGEDeviceContext *RendererSystemD3D11::GetImmediateContext()
{
    return static_cast<PPGEDeviceContext *>(m_device_context_sp.get());
}

PPGESwapChain *RendererSystemD3D11::GetSwapChain()
{
    return static_cast<PPGESwapChain *>(m_swap_chain_sp.get());
}
} // namespace PPGE
