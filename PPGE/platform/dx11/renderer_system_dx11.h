#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "platform/dx11/buffer_dx11.h"
#include "system/logger_system.h"
#include "system/renderer_system.h"

namespace PPGE
{
class RendererContextDX11;

class PPGE_API RendererSystemDX11 : public RendererSystem
{
  public:
    RendererSystemDX11()
    {
        m_device = NULL;
        m_immediate_context = NULL;
        m_swap_chain = NULL;
        m_depth_stencil_buffer = NULL;
        m_depth_stencil_view = NULL;
        m_render_target_view = NULL;

        ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
    }
    ~RendererSystemDX11()
    {
    }

    void StartUp(const RendererSystemProps &props) override;
    void Update() override;
    void ShutDown() override;

    void OnResize() override;

    RendererAPI GetRendererAPI() override
    {
        return RendererAPI::DX11;
    }

    VertexBufferHandle CreateVertexBuffer(const VertexBufferDesc &desc) override;
    void SetVertexBuffer(VertexBufferHandle handle) override;
    void ReleaseVertexBuffer(VertexBufferHandle &handle) override;

    IndexBufferHandle CreateIndexBuffer(const IndexBufferDesc &desc) override;
    void SetIndexBuffer(IndexBufferHandle handle) override;
    void ReleaseIndexBuffer(IndexBufferHandle &handle) override;

    TextureHandle CreateTexture(const TextureDesc &desc) override;
    void SetTexture(TextureHandle handle, Sampler sampler) override;
    void ReleaseTexture(TextureHandle &handle) override;

    UniformHandle CreateUniform(const UniformDesc &desc) override;
    void SetUniform(UniformHandle handle, void *data) override;

    void SetRenderStates(const RenderStates &states) override;

    void Submit(ProgramHandle handle) override;

  private:
    RendererSystemProps m_props;
    UINT m_msaa_quality = 0;
    D3D_DRIVER_TYPE m_driver_type = D3D_DRIVER_TYPE_HARDWARE;

    ID3D11Device *m_device;
    ID3D11DeviceContext *m_immediate_context;
    IDXGISwapChain *m_swap_chain;

    ID3D11Texture2D *m_depth_stencil_buffer;
    ID3D11DepthStencilView *m_depth_stencil_view;
    ID3D11RenderTargetView *m_render_target_view;

    D3D11_VIEWPORT m_viewport;

    VertexBufferD3D11 m_vertex_buffers[PPGE_RENDERER_MAX_VERTEX_BUFFERS];
    IndexBufferD3D11 m_index_buffers[PPGE_RENDERER_MAX_INDEX_BUFFERS];

    static uint16_t s_vertex_buffer_count;
    static uint16_t s_index_buffer_count;

    friend BufferD3D11;
    friend VertexBufferD3D11;
    friend IndexBufferD3D11;
};
} // namespace PPGE