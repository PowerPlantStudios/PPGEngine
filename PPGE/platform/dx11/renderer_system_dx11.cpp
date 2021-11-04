#include "platform/dx11/renderer_system_dx11.h"

#include "system/display_system.h"

namespace PPGE
{
void RendererSystemDX11::StartUp(const RendererSystemProps &props)
{
    CreateDevice();
    CreateContext();
    CreateSwapChain();
    OnResize();
}

void RendererSystemDX11::Update()
{
    m_context->GetD3D11DeviceContextPtr()->ClearRenderTargetView(m_render_target_view, green);
    m_context->GetD3D11DeviceContextPtr()->ClearDepthStencilView(m_depth_stencil_view,
                                                                 D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_swap_chain->Swap();
}

void RendererSystemDX11::ShutDown()
{
    m_swap_chain.reset();
    m_context.reset();
    m_device.reset();
}

void RendererSystemDX11::CreateDevice()
{
    m_device = std::make_unique<DeviceDX11>();
}

void RendererSystemDX11::CreateContext()
{
    PPGE_ASSERT(m_device, "Cannot create context before creating D3D11 device.");
    m_context = std::make_unique<ContextDX11>(m_device->GetD3D11DevicePtr());
}

void RendererSystemDX11::CreateSwapChain()
{
    PPGE_ASSERT(m_device, "Cannot create context before creating D3D11 device.");
    m_swap_chain = std::make_unique<SwapChainDX11>(m_device->GetD3D11DevicePtr(), m_enable_4x_msaa);
}

Context &RendererSystemDX11::GetContext()
{
    return *(m_context.get());
}

SwapChain &RendererSystemDX11::GetSwapChain()
{
    return *(m_swap_chain.get());
}

void RendererSystemDX11::OnResize()
{
    PPGE_ASSERT(m_device, "Render target cannot be initialized before D3D11 device is not created.");
    PPGE_ASSERT(m_context, "Render target cannot be initialized before D3D11 context is not created.");
    PPGE_ASSERT(m_swap_chain, "Render target cannot be initialized before D3D11 swap chain is not initialized.");

    PPGE_RELEASE_COM(m_render_target_view);
    PPGE_RELEASE_COM(m_depth_stencil_view);
    PPGE_RELEASE_COM(m_depth_stencil_buffer);

    PPGE_HR(m_swap_chain->GetD3D11SwapChainPtr()->ResizeBuffers(
        1, DisplaySystem::Get().GetWidth(), DisplaySystem::Get().GetHeight(), DXGI_FORMAT_R8G8B8A8_UNORM, 0));
    ID3D11Texture2D *back_buffer;
    PPGE_HR(m_swap_chain->GetD3D11SwapChainPtr()->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                                            reinterpret_cast<void **>(&back_buffer)));
    PPGE_HR(m_device->GetD3D11DevicePtr()->CreateRenderTargetView(back_buffer, 0, &m_render_target_view));
    PPGE_RELEASE_COM(back_buffer);

    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = DisplaySystem::Get().GetWidth();
    depthStencilDesc.Height = DisplaySystem::Get().GetHeight();
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    if (m_enable_4x_msaa)
    {
        depthStencilDesc.SampleDesc.Count = 4;
        depthStencilDesc.SampleDesc.Quality = m_swap_chain->Get4xMsaaQuality() - 1;
    }
    else
    {
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
    }

    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    PPGE_HR(m_device->GetD3D11DevicePtr()->CreateTexture2D(&depthStencilDesc, 0, &m_depth_stencil_buffer));
    PPGE_HR(m_device->GetD3D11DevicePtr()->CreateDepthStencilView(m_depth_stencil_buffer, 0, &m_depth_stencil_view));

    m_context->GetD3D11DeviceContextPtr()->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);

    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;
    m_viewport.Width = DisplaySystem::Get().GetWidth();
    m_viewport.Height = DisplaySystem::Get().GetHeight();
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;

    m_context->GetD3D11DeviceContextPtr()->RSSetViewports(1, &m_viewport);
}
} // namespace PPGE