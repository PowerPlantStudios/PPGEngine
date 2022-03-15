#include "sampler_d3d11_impl.h"

#include "platform/d3d11/device_d3d11_impl.h"

namespace PPGE
{
SamplerD3D11Impl::SamplerD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, const SamplerDesc &desc)
    : SamplerBaseType(std::move(device_sp), desc)
{
    D3D11_SAMPLER_DESC sd;
    sd.Filter = D3D11_FILTER_ANISOTROPIC;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.MipLODBias = 0.0f;
    sd.MaxAnisotropy = 8;
    sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    sd.BorderColor[0] = 0;
    sd.BorderColor[1] = 0;
    sd.BorderColor[2] = 0;
    sd.BorderColor[3] = 0;
    sd.MinLOD = 0;
    sd.MaxLOD = D3D11_FLOAT32_MAX;

    auto d3d11_device = m_device_sp->GetD3D11Device();
    PPGE_HR(d3d11_device->CreateSamplerState(&sd, &m_d3d11_sampler_state_ptr));
}

SamplerD3D11Impl::~SamplerD3D11Impl()
{
}
} // namespace PPGE
