#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "platform/d3d11/renderer_traits_d3d11.h"
#include "rhi/texture_base.h"

namespace PPGE
{
class TextureD3D11Impl final : public TextureBase<RendererTraitsD3D11>
{
  public:
    using TextureBaseType = TextureBase<RendererTraitsD3D11>;

    TextureD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, const TextureCreateDesc &create_desc);

    TextureD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, ID3D11Texture1D *texture);
    
    TextureD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, ID3D11Texture2D *texture);
    
    TextureD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, ID3D11Texture3D *texture);

    ~TextureD3D11Impl();

    std::shared_ptr<PPGETextureView> CreateView(const TextureViewDesc &desc) override final;

    std::shared_ptr<PPGETextureView> GetDefaultView() override final;

    CComPtr<ID3D11Resource> GetD3D11Resource() const override final
    {
        return m_d3d11_texture_ptr;
    }

    size_t GetGenericHandle() override final
    {
        PPGE_ASSERT(sizeof(m_d3d11_texture_ptr.p) <= sizeof(size_t),
                    "Destination type is too short to cast source type");
        size_t handle = 0;
        memcpy(&handle, &(m_d3d11_texture_ptr.p), sizeof(m_d3d11_texture_ptr.p));
        return handle;
    }

  private:
    void CreateTexture1D(const TextureCreateDesc &create_desc);

    void CreateTexture2D(const TextureCreateDesc &create_desc);

    void CreateTexture3D(const TextureCreateDesc &create_desc);

    std::vector<D3D11_SUBRESOURCE_DATA> CreateInitData(const TextureCreateDesc &create_desc);

    void CreateShaderResourceView(const TextureViewDesc &view_desc, ID3D11ShaderResourceView **d3d11_srv_pp);

    void CreateRenderTargetView(const TextureViewDesc &view_desc, ID3D11RenderTargetView **d3d11_rtv_pp);

    void CreateDepthStencilView(const TextureViewDesc &view_desc, ID3D11DepthStencilView **d3d11_dsv_pp);

    CComPtr<ID3D11Resource> m_d3d11_texture_ptr;
    CComPtr<ID3D11View> m_default_view;
};
} // namespace PPGE
