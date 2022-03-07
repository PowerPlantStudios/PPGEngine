#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "platform/d3d11/shader_resource_variable_d3d11.h"
#include "renderer/shader_resource_variable_base.h"

namespace PPGE
{
class BufferD3D11Impl; 

class ConstantBufferBindingD3D11 final
    : public ShaderResourceVariableBase<PPGEShaderResourceVariableD3D11, ConstantBufferBindingD3D11, BufferD3D11Impl>
{
  public:
    using ShaderResourceVariableBaseType =
        ShaderResourceVariableBase<PPGEShaderResourceVariableD3D11, ConstantBufferBindingD3D11, BufferD3D11Impl>;

    ConstantBufferBindingD3D11(const ShaderResourceDesc &desc) : ShaderResourceVariableBase(desc)
    {
    }

    void BindResource(std::shared_ptr<PPGEDeviceObject> device_object_sp) override final;

    ID3D11DeviceChild *GetD3D11Resource() const override final;
};
} // namespace PPGE