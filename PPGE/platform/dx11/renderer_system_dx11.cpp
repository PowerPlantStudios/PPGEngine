#include "platform/dx11/renderer_system_dx11.h"

#include "system/display_system.h"

namespace PPGE
{
uint16_t RendererSystemDX11::s_vertex_buffer_count = 0;
uint16_t RendererSystemDX11::s_index_buffer_count = 0;

void RendererSystemDX11::StartUp(const RendererSystemProps &props)
{
    UINT device_flags = 0;
#if defined(PPGE_PLATFORM_WIN)
    device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL feature_level;
    HRESULT hr =
        D3D11CreateDevice(0,             // chosen adapter. '0' means default adapter
                          m_driver_type, // driver type to be used
                          0,             // if driver type is software this is handle to software driver
                          device_flags,  // device flags to configure started device
                          0, 0, // feature level array and the size of it. '0' for both means default feature level
                          D3D11_SDK_VERSION, // available sdk version
                          &m_device,         // [out] pointer to device that is created
                          &feature_level, // [out] if feature level array is provided, the first supported feature level
                          &m_immediate_context // [out] poitner to immediate context created
        );
    PPGE_HR(hr);
    PPGE_ASSERT((feature_level != D3D_FEATURE_LEVEL_11_1),
                "PPGE requires DX feature level 11_1 to use DX11 as the rendering API.");

    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = DisplaySystem::Get().GetWidth();
    sd.BufferDesc.Height = DisplaySystem::Get().GetHeight();
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    if (m_enable_4x_msaa)
    {
        PPGE_HR(m_device->CheckMultisampleQualityLevels(
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
    PPGE_HR(m_device->QueryInterface(__uuidof(IDXGIDevice), (void **)&dxgi_device));
    IDXGIAdapter *dxgi_adapter = NULL;
    PPGE_HR(dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void **)&dxgi_adapter));
    IDXGIFactory *dxgi_factory = NULL;
    PPGE_HR(dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void **)&dxgi_factory));

    PPGE_HR(dxgi_factory->CreateSwapChain(m_device, &sd, &m_swap_chain));

    PPGE_RELEASE_COM(dxgi_device);
    PPGE_RELEASE_COM(dxgi_adapter);
    PPGE_RELEASE_COM(dxgi_factory);

    OnResize();
}

void RendererSystemDX11::Update()
{
    // m_immediate_context->ClearRenderTargetView(m_render_target_view, green);
    // m_immediate_context->ClearDepthStencilView(m_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f,
    // 0);

    m_swap_chain->Present(0, 0);
}

void RendererSystemDX11::ShutDown()
{
    PPGE_RELEASE_COM(m_render_target_view);
    PPGE_RELEASE_COM(m_depth_stencil_view);
    PPGE_RELEASE_COM(m_depth_stencil_buffer);

    PPGE_RELEASE_COM(m_swap_chain);
    PPGE_RELEASE_COM(m_immediate_context);
    PPGE_RELEASE_COM(m_device);

    s_vertex_buffer_count = 0;
    s_index_buffer_count = 0;
}

VertexBufferHandle RendererSystemDX11::CreateVertexBuffer(const VertexBufferDesc &desc)
{
    VertexBufferHandle handle;
    m_vertex_buffers[s_vertex_buffer_count] = VertexBufferD3D11();
    handle.idx = s_vertex_buffer_count++;
    return handle;
}

IndexBufferHandle RendererSystemDX11::CreateIndexBuffer(const IndexBufferDesc &desc)
{
    return IndexBufferHandle();
}

TextureHandle RendererSystemDX11::CreateTexture(const TextureDesc &desc)
{
    return TextureHandle();
}

ShaderHandle RendererSystemDX11::CreateShader(const ShaderDesc &desc)
{
    return ShaderHandle();
}

void RendererSystemDX11::ReleaseVertexBuffer(VertexBufferHandle &hnd)
{
}

void RendererSystemDX11::ReleaseIndexBuffer(IndexBufferHandle &hnd)
{
}

void RendererSystemDX11::ReleaseTexture(TextureHandle &hnd)
{
}

void RendererSystemDX11::ReleaseShader(ShaderHandle &hnd)
{
}

void RendererSystemDX11::LoadVertexBuffer(const VertexBufferHandle &hnd)
{
}

void RendererSystemDX11::UnloadVertexBuffer(const VertexBufferHandle &hnd)
{
}

void RendererSystemDX11::LoadIndexBuffer(const IndexBufferHandle &hnd)
{
}

void RendererSystemDX11::UnloadIndexBuffer(const IndexBufferHandle &hnd)
{
}

void RendererSystemDX11::LoadTexture(const TextureHandle &hnd)
{
}

void RendererSystemDX11::UnloadTexture(const TextureHandle &hnd)
{
}

void RendererSystemDX11::LoadShader(const ShaderHandle &hnd)
{
}

void RendererSystemDX11::UnloadShader(const ShaderHandle &hnd)
{
}

void RendererSystemDX11::OnResize()
{
    if (m_device == NULL)
        return;

    PPGE_RELEASE_COM(m_render_target_view);
    PPGE_RELEASE_COM(m_depth_stencil_view);
    PPGE_RELEASE_COM(m_depth_stencil_buffer);

    PPGE_HR(m_swap_chain->ResizeBuffers(1, DisplaySystem::Get().GetWidth(), DisplaySystem::Get().GetHeight(),
                                        DXGI_FORMAT_R8G8B8A8_UNORM, 0));
    ID3D11Texture2D *back_buffer;
    PPGE_HR(m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&back_buffer)));
    PPGE_HR(m_device->CreateRenderTargetView(back_buffer, 0, &m_render_target_view));
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
        depthStencilDesc.SampleDesc.Quality = m_4x_msaa_quality - 1;
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

    PPGE_HR(m_device->CreateTexture2D(&depthStencilDesc, 0, &m_depth_stencil_buffer));
    PPGE_HR(m_device->CreateDepthStencilView(m_depth_stencil_buffer, 0, &m_depth_stencil_view));

    m_immediate_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);

    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;
    m_viewport.Width = DisplaySystem::Get().GetWidth();
    m_viewport.Height = DisplaySystem::Get().GetHeight();
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;

    m_immediate_context->RSSetViewports(1, &m_viewport);
}
void RendererSystemDX11::Submit()
{
}
} // namespace PPGE