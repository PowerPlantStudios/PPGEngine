#include "texture_view_d3d11_impl.h"

#include "platform/d3d11/texture_d3d11_impl.h"

namespace PPGE
{
TextureViewD3D11Impl::TextureViewD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp,
                                           std::shared_ptr<TextureD3D11Impl> texture_sp, ID3D11View *d3d11_view_ptr,
                                           const TextureViewDesc &desc)
    : TextureViewBaseType(std::move(device_sp), std::move(texture_sp), desc), m_d3d11_view_ptr{d3d11_view_ptr}
{
}

TextureViewD3D11Impl::~TextureViewD3D11Impl()
{
}
} // namespace PPGE
