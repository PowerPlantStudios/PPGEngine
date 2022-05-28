#include "sampler_d3d11_impl.h"

#include "platform/d3d11/device_d3d11_impl.h"

namespace PPGE
{
SamplerD3D11Impl::SamplerD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, const SamplerDesc &desc)
    : SamplerBaseType(std::move(device_sp), desc)
{
    D3D11_SAMPLER_DESC sd;
    sd.Filter = PPGEFilterTypeToD3D11FilterType(m_desc.min_filter, m_desc.mag_filter, m_desc.mip_filter);
    sd.AddressU = PPGETextureAddressTypeToD3D11TextureAddressType(m_desc.address_u);
    sd.AddressV = PPGETextureAddressTypeToD3D11TextureAddressType(m_desc.address_v);
    sd.AddressW = PPGETextureAddressTypeToD3D11TextureAddressType(m_desc.address_w);
    sd.MipLODBias = m_desc.mip_lod_bias;
    sd.MaxAnisotropy = m_desc.max_anisotropy;
    sd.ComparisonFunc = PPGEComperisonFuncTypeToD3D11ComparisonFunc(m_desc.comperison_func);
    sd.BorderColor[0] = m_desc.border_color[0];
    sd.BorderColor[1] = m_desc.border_color[1];
    sd.BorderColor[2] = m_desc.border_color[2];
    sd.BorderColor[3] = m_desc.border_color[3];
    sd.MinLOD = m_desc.min_lod;
    sd.MaxLOD = m_desc.max_lod;

    auto d3d11_device = m_device_sp->GetD3D11Device();
    PPGE_HR(d3d11_device->CreateSamplerState(&sd, &m_d3d11_sampler_state_ptr));
}

SamplerD3D11Impl::~SamplerD3D11Impl()
{
}
} // namespace PPGE
