#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "platform/d3d11/buffer_d3d11.h"
#include "platform/d3d11/texture_d3d11.h"
#include "platform/d3d11/shader_d3d11.h"
#include "platform/d3d11/vertex_layout_d3d11.h"
#include "platform/d3d11/imgui_widget_d3d11.h"
#include "system/logger_system.h"
#include "system/renderer_system.h"

namespace PPGE
{
class PPGE_API RendererSystemD3D11 : public RendererSystem
{
  public:
    RendererSystemD3D11()
    {
        m_device = NULL;
        m_immediate_context = NULL;
        m_swap_chain = NULL;
        m_depth_stencil_buffer = NULL;
        m_depth_stencil_view = NULL;
        m_render_target_view = NULL;

        ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
    }
    ~RendererSystemD3D11()
    {
    }

    void StartUp(const RendererSystemProps &props) override;
    void Update() override;
    void ShutDown() override;

    void OnResize() override;

    RendererAPI GetRendererAPI() override
    {
        return RendererAPI::D3D11;
    }

    bool ClearColor(float r, float g, float b) override;
    bool ClearDepthStencilBuffer(float depth, uint8_t stencil) override;

    bool CreateVertexBuffer(const VertexBufferDesc &desc, VertexBufferHandle handle) override;
    bool ReleaseVertexBuffer(VertexBufferHandle handle) override;

    bool CreateVertexLayout(const VertexLayout &layout, VertexLayoutHandle handle) override;
    bool ReleaseVertexLayout(VertexLayoutHandle handle) override;

    bool CreateIndexBuffer(const IndexBufferDesc &desc, IndexBufferHandle handle) override;
    bool ReleaseIndexBuffer(IndexBufferHandle handle) override;

    bool CreateTexture(const TextureDesc &desc, TextureHandle handle) override;
    bool ReleaseTexture(TextureHandle handle) override;

    bool CreateProgram(const ProgramDesc &desc, ProgramHandle handle) override;
    bool ReleaseProgram(ProgramHandle handle) override;

    bool CreateShader(const ShaderDesc &desc, ShaderHandle handle) override;
    bool ReleaseShader(ShaderHandle handle) override;

    bool CreateUniform(const UniformDesc &desc, UniformHandle handle) override;
    bool ReleaseUniform(UniformHandle handle) override;

    bool SetPredefinedUniform(const PredefinedUniform &uniform) override;
    bool SetRenderStates(const RenderStates &states) override;

    bool Submit(const Frame &frame) override;

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

    std::array<VertexBufferD3D11, PPGE_RENDERER_MAX_VERTEX_BUFFERS> m_vertex_buffers;
    std::array<VertexLayoutD3D11, PPGE_RENDERER_MAX_VERTEX_LAYOUTS> m_vertex_layouts;
    std::array<IndexBufferD3D11, PPGE_RENDERER_MAX_INDEX_BUFFERS> m_index_buffers;
    std::array<ShaderD3D11, PPGE_RENDERER_MAX_SHADERS> m_shaders;
    std::array<BufferD3D11, PPGE_RENDERER_MAX_UNIFORMS> m_uniforms;
    std::array<BufferD3D11, PPGE_RENDERER_PREDEFINED_UNIFORMS> m_predefined_uniforms;
    std::array<ProgramD3D11, PPGE_RENDERER_MAX_PROGRAMS> m_programs;

    VertexBufferHandle m_current_vb;
    IndexBufferHandle m_current_ib;
    VertexLayoutHandle m_current_ly;
    ProgramHandle m_current_pg;
    D3D11_PRIMITIVE_TOPOLOGY m_current_topology;

    friend BufferD3D11;
    friend VertexBufferD3D11;
    friend VertexLayoutD3D11;
    friend IndexBufferD3D11;
    friend TextureD3D11;
    friend ShaderD3D11;
    friend ProgramD3D11;
    friend ImGuiWidgetD3D11;
};
} // namespace PPGE