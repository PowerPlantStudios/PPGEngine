#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/shader.h"

namespace PPGE
{
inline constexpr uint8_t D3D11_Num_of_Shaders = 6;

class PPGE_API PPGEShaderD3D11 : public PPGEShader
{
  public:
    virtual CComPtr<ID3DBlob> GetByteCode() const = 0;

    virtual CComPtr<ID3D11DeviceChild> GetD3D11Shader() const = 0;
};
} // namespace PPGE
