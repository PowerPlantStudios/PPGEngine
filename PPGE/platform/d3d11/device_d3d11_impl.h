#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "platform/d3d11/renderer_traits_d3d11.h"
#include "renderer/device_base.h"

namespace PPGE
{
class DeviceD3D11Impl final : public DeviceBase<RendererTraitsD3D11>
{
  public:
    using DeviceBaseType = DeviceBase<RendererTraitsD3D11>;

    DeviceD3D11Impl(ID3D11Device1 *d3d11_device_ptr) : DeviceBaseType(), m_d3d11_device_ptr{d3d11_device_ptr}
    {
    }

    ~DeviceD3D11Impl()
    {
    }

    void CreateBuffer(const BufferDesc &desc, const BufferData *data,
                      std::shared_ptr<PPGEBuffer> &buffer_sp) override final;

    void CreatePipelineState(const GfxPipelineStateCreateDesc &desc,
                             std::shared_ptr<PPGEPipelineState> &PSO_sp) override final;

    void CreateShader(const ShaderCreateDesc &desc, std::shared_ptr<PPGEShader> &shader_sp) override final;

    void CreateTexture(const TextureCreateDesc &desc, std::shared_ptr<PPGETexture> &texture_sp) override final;

    void CreateTextureFromD3D11Resource(ID3D11Texture1D *texture_ptr, std::shared_ptr<PPGETexture> &texture_sp);

    void CreateTextureFromD3D11Resource(ID3D11Texture2D *texture_ptr, std::shared_ptr<PPGETexture> &texture_sp);

    void CreateTextureFromD3D11Resource(ID3D11Texture3D *texture_ptr, std::shared_ptr<PPGETexture> &texture_sp);

    void CreateSampler(const SamplerDesc &desc, std::shared_ptr<PPGESampler> &sampler_sp) override final;

    CComPtr<ID3D11Device> GetD3D11Device() const override final
    {
        return m_d3d11_device_ptr;
    }

  private:
    CComPtr<ID3D11Device> m_d3d11_device_ptr;
};
} // namespace PPGE