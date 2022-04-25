#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/pipeline_state.h"

namespace PPGE
{
class PPGE_API PPGEPipelineStateD3D11 : public PPGEPipelineState
{
  public:
    virtual ID3D11BlendState *GetD3D11BlendStateRaw() const = 0;

    virtual ID3D11RasterizerState *GetD3D11RasterizerStateRaw() const = 0;

    virtual ID3D11DepthStencilState *GetD3D11DepthStencilStateRaw() const = 0;

    virtual ID3D11InputLayout *GetD3D11InputLayoutRaw() const = 0;

    virtual CComPtr<ID3D11VertexShader> GetD3D11VertexShaderShared() const = 0;

    virtual ID3D11VertexShader *GetD3D11VertexShaderRaw() const = 0;

    virtual CComPtr<ID3D11PixelShader> GetD3D11PixelShaderShared() const = 0;

    virtual ID3D11PixelShader *GetD3D11PixelShaderRaw() const = 0;

    virtual CComPtr<ID3D11GeometryShader> GetD3D11GeometryShaderShared() const = 0;

    virtual ID3D11GeometryShader *GetD3D11GeometryShaderRaw() const = 0;

    virtual CComPtr<ID3D11DomainShader> GetD3D11DomainShaderShared() const = 0;

    virtual ID3D11DomainShader *GetD3D11DomainShaderRaw() const = 0;

    virtual CComPtr<ID3D11HullShader> GetD3D11HullShaderShared() const = 0;

    virtual ID3D11HullShader *GetD3D11HullShaderRaw() const = 0;

    virtual CComPtr<ID3D11ComputeShader> GetD3D11ComputeShaderShared() const = 0;

    virtual ID3D11ComputeShader *GetD3D11ComputeShaderRaw() const = 0;
};
} // namespace PPGE
