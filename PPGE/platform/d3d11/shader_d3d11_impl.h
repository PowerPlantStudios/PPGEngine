#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "platform/d3d11/renderer_traits_d3d11.h"
#include "platform/d3d11/renderer_types_d3d11_conversion.h"
#include "renderer/shader_base.h"

namespace PPGE
{
class ShaderD3D11Impl final : public ShaderBase<RendererTraitsD3D11>
{
  public:
    using ShaderBaseType = ShaderBase<RendererTraitsD3D11>;

    ShaderD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, const ShaderCreateDesc &create_desc);

    ~ShaderD3D11Impl();

    CComPtr<ID3DBlob> GetByteCode() const override final
    {
        return m_shader_byte_code_ptr;
    }

    CComPtr<ID3D11DeviceChild> GetD3D11Shader() const override final
    {
        return m_d3d11_shader_ptr;
    }

  private:
    CComPtr<ID3D11DeviceChild> m_d3d11_shader_ptr;
    CComPtr<ID3DBlob> m_shader_byte_code_ptr;

};
} // namespace PPGE