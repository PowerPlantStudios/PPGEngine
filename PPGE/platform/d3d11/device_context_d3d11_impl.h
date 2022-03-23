#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "platform/d3d11/renderer_traits_d3d11.h"
#include "renderer/device_context_base.h"

namespace PPGE
{
class DeviceContextD3D11Impl final : public DeviceContextBase<RendererTraitsD3D11>
{
  public:
    using DeviceContextBaseType = DeviceContextBase<RendererTraitsD3D11>;

    DeviceContextD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, ID3D11DeviceContext1 *d3d11_device_context_ptr,
                           const DeviceContextDesc &desc)
        : DeviceContextBaseType(std::move(device_sp), desc), m_d3d11_device_context_ptr{d3d11_device_context_ptr}
    {
    }

    ~DeviceContextD3D11Impl()
    {
    }

    void SetVertexBuffers(uint32_t num_buffers, std::shared_ptr<PPGEBuffer> p_buffers[],
                          const uint64_t offsets[]) override final;

    void SetIndexBuffer(std::shared_ptr<PPGEBuffer> p_buffer) override final;

    void SetPipelineStateObject(std::shared_ptr<PPGEPipelineState> p_PSO) override final;

    void CommitShaderResources(const std::shared_ptr<PPGEShaderResourceBinding> &p_SRB) override final;

    void SetViewports(uint32_t num_viewports, const Viewport viewports[]) override final;

    void SetScissorRects(uint32_t num_rects, const Rect rects[]) override final;

    void SetRenderTargets(uint32_t num_render_targets, std::shared_ptr<PPGETextureView> pp_RTs[],
                          std::shared_ptr<PPGETextureView> p_DS) override final;

    void ClearRenderTarget(const std::shared_ptr<PPGETextureView> &p_view, float r, float b, float g,
                           float a) override final;

    void ClearDepthStencil(const std::shared_ptr<PPGETextureView> &p_view, float d, uint8_t s) override final;

    void Draw(uint32_t num_vertices) override final;

    void DrawIndirect(uint32_t num_vertices) override final;

    void DrawIndexed(uint32_t num_indicies) override final;
    
    void DrawIndexedIndirect(uint32_t num_indicies) override final;

    void Map(PPGEBuffer *p_buffer, MapType map_type, MapFlags map_flags, void **pp_map_data) override final;

    void Unmap(PPGEBuffer *p_buffer) override final;

    void Flush() override final;

    CComPtr<ID3D11DeviceContext> GetD3D11DeviceContext() const override final
    {
        return m_d3d11_device_context_ptr;
    }

  private:
    CComPtr<ID3D11DeviceContext> m_d3d11_device_context_ptr;

    std::array<bool, D3D11_Num_of_Shaders> b_is_shader_bound = {false, false, false, false, false, false};
    
    std::array<CComPtr<ID3D11DeviceChild>, D3D11_Num_of_Shaders> m_bound_d3d11_shaders_ptr{};

    std::array<ID3D11Buffer *, Num_Of_Buffer_Slots> m_bound_d3d11_vertex_buffers_ptr{};

    std::array<UINT, Num_Of_Buffer_Slots> m_bound_d3d11_vertex_buffer_strides{};

    std::array<UINT, Num_Of_Buffer_Slots> m_bound_d3d11_vertex_buffer_offsets{};

    CComPtr<ID3D11Buffer> m_bound_d3d11_index_buffer_ptr;

    ID3D11InputLayout *m_input_layout_ptr;

    D3D11_PRIMITIVE_TOPOLOGY m_bound_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
};
} // namespace PPGE