#include "platform/dx11/renderer_system_dx11.h"

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

static size_t GetUniformSize(UniformDesc::UniformType type)
{
    switch (type)
    {
    case PPGE::UniformDesc::UniformType::Scalar:
        return 4;
    case PPGE::UniformDesc::UniformType::Float2:
        return 8;
    case PPGE::UniformDesc::UniformType::Float3:
        return 16;
    case PPGE::UniformDesc::UniformType::Float4:
        return 16;
    case PPGE::UniformDesc::UniformType::Mat3:
        return 64;
    case PPGE::UniformDesc::UniformType::Mat4:
        return 64;
    default:
        PPGE_ASSERT(false, "Uniform type is unknown.");
        return 0;
    }
}

void RendererSystemDX11::StartUp(const RendererSystemProps &props)
{
    m_props = props;

    UINT device_flags = 0;
#if defined(PPGE_DEBUG)
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

    if (m_props.msaa_quality != MSAAQuality::MSAA_OFF)
    {
        PPGE_HR(m_device->CheckMultisampleQualityLevels(
            DXGI_FORMAT_R8G8B8A8_UNORM, // Render target format to check quality level for the given sample count
            GetMSAASampleCount(m_props.msaa_quality), // Sample count for MSAA
            &m_msaa_quality // [out] Supported max quality level for the given render target format and sample count
            ));
        // Sample count 4 should be supported for all render targets for DX11 capable devices
        PPGE_ASSERT((m_msaa_quality > 0), "For the given render target and sample count, MSAA is not supported.");

        sd.SampleDesc.Count = GetMSAASampleCount(m_props.msaa_quality);
        sd.SampleDesc.Quality = m_msaa_quality - 1;
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

    m_vertex_buffers.fill(VertexBufferD3D11());
    m_vertex_layouts.fill(VertexLayoutD3D11());
    m_index_buffers.fill(IndexBufferD3D11());
    m_shaders.fill(ShaderD3D11());
    m_uniforms.fill(BufferD3D11());
    m_predefined_uniforms.fill(BufferD3D11());
    m_programs.fill(ProgramD3D11());
}

void RendererSystemDX11::Update()
{
    m_swap_chain->Present(1, 0);
}

void RendererSystemDX11::ShutDown()
{
    for (auto &vb : m_vertex_buffers)
        vb.Destroy();

    for (auto &ly : m_vertex_layouts)
        ly.Destroy();

    for (auto &ib : m_index_buffers)
        ib.Destroy();

    for (auto &sh : m_shaders)
        sh.Destroy();

    for (auto &cb : m_uniforms)
        cb.Destroy();

    PPGE_RELEASE_COM(m_render_target_view);
    PPGE_RELEASE_COM(m_depth_stencil_view);
    PPGE_RELEASE_COM(m_depth_stencil_buffer);

    PPGE_RELEASE_COM(m_swap_chain);
    PPGE_RELEASE_COM(m_immediate_context);
    PPGE_RELEASE_COM(m_device);
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

    if (m_props.msaa_quality != MSAAQuality::MSAA_OFF)
    {
        depthStencilDesc.SampleDesc.Count = GetMSAASampleCount(m_props.msaa_quality);
        depthStencilDesc.SampleDesc.Quality = m_msaa_quality - 1;
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

bool RendererSystemDX11::ClearColor(float r, float g, float b)
{
    float clear_color[] = {r, g, b, 1.0f};
    m_immediate_context->ClearRenderTargetView(m_render_target_view, clear_color);
    return true;
}

bool RendererSystemDX11::ClearDepthStencilBuffer(float depth, uint8_t stencil)
{
    UINT clear_flag = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL;
    m_immediate_context->ClearDepthStencilView(m_depth_stencil_view, clear_flag, depth, stencil);
    return true;
}

bool RendererSystemDX11::CreateVertexBuffer(const VertexBufferDesc &desc, VertexBufferHandle handle)
{
    VertexBufferD3D11 &vertex_buffer = m_vertex_buffers[handle.idx];
    return vertex_buffer.Create(desc);
}

bool RendererSystemDX11::ReleaseVertexBuffer(VertexBufferHandle handle)
{
    VertexBufferD3D11 &vertex_buffer = m_vertex_buffers[handle.idx];
    vertex_buffer.Destroy();
    return true;
}

bool RendererSystemDX11::CreateVertexLayout(const VertexLayout &layout, VertexLayoutHandle handle)
{
    VertexLayoutD3D11 &vertex_layout = m_vertex_layouts[handle.idx];
    vertex_layout.SetLayout(layout);
    return true;
}

bool RendererSystemDX11::ReleaseVertexLayout(VertexLayoutHandle handle)
{
    VertexLayoutD3D11 &vertex_layout = m_vertex_layouts[handle.idx];
    vertex_layout.Destroy();
    return true;
}

bool RendererSystemDX11::CreateIndexBuffer(const IndexBufferDesc &desc, IndexBufferHandle handle)
{
    IndexBufferD3D11 &index_buffer = m_index_buffers[handle.idx];
    return index_buffer.Create(desc);
}

bool RendererSystemDX11::ReleaseIndexBuffer(IndexBufferHandle handle)
{
    IndexBufferD3D11 &index_buffer = m_index_buffers[handle.idx];
    index_buffer.Destroy();
    return true;
}

bool RendererSystemDX11::CreateTexture(const TextureDesc &desc, TextureHandle handle)
{
    return false;
}

bool RendererSystemDX11::ReleaseTexture(TextureHandle handle)
{
    return false;
}

bool RendererSystemDX11::CreateProgram(const ProgramDesc &desc, ProgramHandle handle)
{
    ProgramD3D11 &program = m_programs[handle.idx];
    return program.Create(desc);
}

bool RendererSystemDX11::ReleaseProgram(ProgramHandle handle)
{
    ProgramD3D11 &program = m_programs[handle.idx];
    program.Destroy();
    return true;
}

bool RendererSystemDX11::CreateShader(const ShaderDesc &desc, ShaderHandle handle)
{
    ShaderD3D11 &shader = m_shaders[handle.idx];
    return shader.Create(desc);
}

bool RendererSystemDX11::ReleaseShader(ShaderHandle handle)
{
    ShaderD3D11 &shader = m_shaders[handle.idx];
    shader.Destroy();
    return true;
}

bool RendererSystemDX11::CreateUniform(const UniformDesc &desc, UniformHandle handle)
{
    BufferD3D11 &c_buffer = m_uniforms[handle.idx];

    BufferDesc bd;
    bd.m_cpu_flags = BufferDesc::CPUFlag::Write;
    bd.m_usage = BufferDesc::UsageType::Dynamic;
    bd.m_resource.m_size = GetUniformSize(desc.m_type) * desc.m_num;

    return c_buffer.Create(bd);
}

bool RendererSystemDX11::ReleaseUniform(UniformHandle handle)
{
    BufferD3D11 &c_buffer = m_uniforms[handle.idx];
    c_buffer.Destroy();
    return true;
}

bool RendererSystemDX11::SetPredefinedUniform(const PredefinedUniform &uniform)
{
    BufferD3D11 &c_buffer = m_predefined_uniforms[uniform.handle.idx];
    if (c_buffer.GetBufferSize() < uniform.subresource.m_size)
    {
        BufferDesc bd;
        bd.m_cpu_flags = BufferDesc::CPUFlag::Write;
        bd.m_usage = BufferDesc::UsageType::Dynamic;
        bd.m_resource.m_size = uniform.subresource.m_size;
        if (!c_buffer.Create(bd))
            return false;
    }

    c_buffer.Update(uniform.subresource);
    c_buffer.Set(uniform.target, uniform.slot);

    return true;
}

bool RendererSystemDX11::SetRenderStates(const RenderStates &states)
{
    return false;
}

bool RendererSystemDX11::Submit(const Frame &frame)
{
    for (const auto &draw_data : frame)
    {
        VertexBufferHandle vb_handle = draw_data.vb_handle;
        PPGE_ASSERT(vb_handle.IsValid(), "Invalid handle to vertex buffer is passed.");

        VertexLayoutHandle ly_handle = draw_data.ly_handle;
        PPGE_ASSERT(ly_handle.IsValid(), "Invalid handle to vertex layout is attached to vertex buffer.");

        IndexBufferHandle ib_handle = draw_data.ib_handle;
        PPGE_ASSERT(ib_handle.IsValid(), "Invalid handle to index buffer is passed.");

        ProgramHandle pg_handle = draw_data.pg_handle;
        PPGE_ASSERT(pg_handle.IsValid(), "Invalid handle to program is passed.");

        for (uint8_t i = 0; i < draw_data.un_update_count; ++i)
        {
            const UniformUpdate &un_update = draw_data.un_updates[i];
            UniformHandle un_handle = un_update.un_handle;
            PPGE_ASSERT(un_handle.IsValid(), "Invalid handle to uniform is passed.");

            BufferD3D11 &c_buffer = m_uniforms[un_handle.idx];
            c_buffer.Update(un_update.subresource);
        }

        for (uint8_t i = 0; i < draw_data.un_bind_count; ++i)
        {
            const UniformBind &un_bind = draw_data.un_binds[i];
            UniformHandle un_handle = un_bind.un_handle;
            PPGE_ASSERT(un_handle.IsValid(), "Invalid handle to uniform is passed.");

            BufferD3D11 &c_buffer = m_uniforms[un_handle.idx];
            c_buffer.Set(un_bind.target, un_bind.slot);
        }

        if (m_current_vb != vb_handle)
        {
            VertexBufferD3D11 &vb = m_vertex_buffers[vb_handle.idx];
            vb.Set();
            m_current_vb = vb_handle;
        }

        if (m_current_ib != ib_handle)
        {
            IndexBufferD3D11 &ib = m_index_buffers[ib_handle.idx];
            ib.Set();
            m_current_ib = ib_handle;
        }

        if (m_current_pg != pg_handle)
        {
            ProgramD3D11 &pg = m_programs[pg_handle.idx];

            ShaderHandle vs_handle = pg.GetVSHandle();
            PPGE_ASSERT(vs_handle.IsValid(), "Invalid handle to vertex shader is obtained.");
            ShaderD3D11 &vs = m_shaders[vs_handle.idx];
            vs.Set();

            ShaderHandle ps_handle = pg.GetPSHandle();
            PPGE_ASSERT(ps_handle.IsValid(), "Invalid handle to pixel shader is obtained.");
            ShaderD3D11 &ps = m_shaders[ps_handle.idx];
            ps.Set();

            VertexLayoutD3D11 &ly = m_vertex_layouts[ly_handle.idx];
            PPGE_ASSERT(ly.HasLayoutDesc(), "Vertex layout is not initialized");

            if (m_current_ly != ly_handle)
            {
                if (!ly.IsCreated())
                    ly.Create(vs);
                ly.Set();
                m_current_ly = ly_handle;
            }

            m_current_pg = pg_handle;
        }

        if (m_current_topology != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
        {
            m_immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            m_current_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        }
        IndexBufferD3D11 &ib = m_index_buffers[m_current_ib.idx];
        m_immediate_context->DrawIndexed(ib.GetIndexCount(), 0, 0);
    }

    return true;
}

} // namespace PPGE