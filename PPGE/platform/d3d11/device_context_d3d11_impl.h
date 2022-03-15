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

    void SetVertexBuffers(uint32_t num_buffers, PPGEBuffer *p_buffer) override final;

    void SetIndexBuffer(PPGEBuffer *p_buffer) override final;

    void SetPipelineObject(PPGEPipelineState *p_PSO) override final;

    void SetShaderResourceBinding(PPGEShaderResourceBinding *p_SRB) override final;

    // void SetRenderTargets(uint32_t num_render_targets, ITextureView **pp_RTs, ITextureView *p_DS)  override final;
    // void ClearRenderTarget(ITextureView* p_view, float r, float b, float g, float a) override final;
    // void ClearDepthStencil(ITextureView* p_view, float d, uint8_t s) override final;

    void Draw(uint32_t num_vertices) override final;

    void DrawIndexed(uint32_t num_indicies) override final;

    void Map(PPGEBuffer *p_buffer, MapType map_type, MapFlags map_flags, void **pp_map_data) override final;
    
    void Unmap(PPGEBuffer *p_buffer) override final;

    CComPtr<ID3D11DeviceContext> GetD3D11DeviceContext() const override final
    {
        return m_d3d11_device_context_ptr;
    }

  private:
    std::array<bool, D3D11_Num_of_Shaders> b_is_shader_bound = {false, false, false, false, false, false};
    CComPtr<ID3D11DeviceContext> m_d3d11_device_context_ptr;
};
} // namespace PPGE