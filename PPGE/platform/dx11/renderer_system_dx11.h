#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
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
        m_depth_stencil_buffer = NULL;
        m_depth_stencil_view = NULL;
        m_render_target_view = NULL;
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

    RendererAPI GetRendererAPI() override
    {
        return RendererAPI::DX11;
    }

    VertexBufferHandle CreateVertexBuffer(const VertexBufferDesc &desc) override;
    IndexBufferHandle CreateIndexBuffer(const IndexBufferDesc &desc) override;
    TextureHandle CreateTexture(const TextureDesc &desc) override;
    ShaderHandle CreateShader(const ShaderDesc &desc) override;

    void ReleaseVertexBuffer(VertexBufferHandle &hnd) override;
    void ReleaseIndexBuffer(IndexBufferHandle &hnd) override;
    void ReleaseTexture(TextureHandle &hnd) override;
    void ReleaseShader(ShaderHandle &hnd) override;

    void LoadVertexBuffer(const VertexBufferHandle &hnd) override;
    void UnloadVertexBuffer(const VertexBufferHandle &hnd) override;
    void LoadIndexBuffer(const IndexBufferHandle &hnd) override;
    void UnloadIndexBuffer(const IndexBufferHandle &hnd) override;
    void LoadTexture(const TextureHandle &hnd) override;
    void UnloadTexture(const TextureHandle &hnd) override;
    void LoadShader(const ShaderHandle &hnd) override;
    void UnloadShader(const ShaderHandle &hnd) override;

    void OnResize() override;

    void Submit() override;

  private:
    PPGE::Math::Color green = PPGE::Math::Color(0.15f, 0.15f, 0.15f);
    
    bool m_enable_4x_msaa = true;
    UINT m_4x_msaa_quality = 0;

    D3D_DRIVER_TYPE m_driver_type = D3D_DRIVER_TYPE_HARDWARE;

    ID3D11Device *m_device;
    ID3D11DeviceContext *m_immediate_context;
    IDXGISwapChain *m_swap_chain;

    ID3D11Texture2D *m_depth_stencil_buffer;
    ID3D11DepthStencilView *m_depth_stencil_view;
    ID3D11RenderTargetView *m_render_target_view;

    D3D11_VIEWPORT m_viewport;
};
} // namespace PPGE