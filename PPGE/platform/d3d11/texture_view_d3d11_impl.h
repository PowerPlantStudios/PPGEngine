#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "platform/d3d11/renderer_traits_d3d11.h"
#include "renderer/texture_view_base.h"

namespace PPGE
{
class TextureViewD3D11Impl final : public TextureViewBase<RendererTraitsD3D11>
{
  public:
    using TextureViewBaseType = TextureViewBase<RendererTraitsD3D11>;

    TextureViewD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, std::shared_ptr<TextureD3D11Impl> texture_sp,
                         ID3D11View *d3d11_view_ptr, const TextureViewDesc &desc);

    ~TextureViewD3D11Impl();

    std::shared_ptr<PPGETexture> GetTexture() const override final;

    CComPtr<ID3D11View> GetD3D11View() const override final
    {
        return m_d3d11_view_ptr;
    }

  private:
    CComPtr<ID3D11View> m_d3d11_view_ptr;
};
} // namespace PPGE