#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "platform/dx11/buffer_dx11.h"
#include "system/logger_system.h"
#include "system/renderer_system.h"

namespace PPGE
{
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

    bool CreateVertexBuffer(const VertexBufferDesc &desc, VertexBufferHandle handle) override;
    bool SetVertexBuffer(VertexBufferHandle handle) override;
    bool ReleaseVertexBuffer(VertexBufferHandle handle) override;

    bool CreateIndexBuffer(const IndexBufferDesc &desc, IndexBufferHandle handle) override;
    bool SetIndexBuffer(IndexBufferHandle handle) override;
    bool ReleaseIndexBuffer(IndexBufferHandle handle) override;

    bool CreateTexture(const TextureDesc &desc, TextureHandle handle) override;
    bool SetTexture(TextureHandle handle, Sampler sampler) override;
    bool ReleaseTexture(TextureHandle handle) override;

    bool CreateUniform(const UniformDesc &desc, UniformHandle handle) override;
    bool SetUniform(UniformHandle handle, void *data) override;

    bool SetRenderStates(const RenderStates &states) override;

    bool Submit(ProgramHandle handle) override;

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

    friend BufferD3D11;
    friend VertexBufferD3D11;
    friend IndexBufferD3D11;
    friend class ImGuiLayerDX11;
};
} // namespace PPGE