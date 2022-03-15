#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "platform/d3d11/renderer_traits_d3d11.h"
#include "platform/d3d11/renderer_types_d3d11_conversion.h"
#include "renderer/sampler_base.h"

namespace PPGE
{
class SamplerD3D11Impl final : public SamplerBase<RendererTraitsD3D11>
{
  public:
    using SamplerBaseType = SamplerBase<RendererTraitsD3D11>;

    SamplerD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, const SamplerDesc &desc);

    ~SamplerD3D11Impl();

    CComPtr<ID3D11SamplerState> GetD3D11SamplerState() const override final
    {
        return m_d3d11_sampler_state_ptr;
    }

  private:
    CComPtr<ID3D11SamplerState> m_d3d11_sampler_state_ptr;
};
} // namespace PPGE