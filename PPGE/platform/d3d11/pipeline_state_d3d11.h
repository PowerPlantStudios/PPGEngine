#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/pipeline_state.h"

namespace PPGE
{
class PPGE_API PPGEPipelineStateD3D11 : public PPGEPipelineState
{
  public:
    virtual ID3D11InputLayout *GetD3D11InputLayout() const = 0;

    virtual ID3D11VertexShader *GetD3D11VertexShader() const = 0;

    virtual ID3D11PixelShader *GetD3D11PixelShader() const = 0;
    
    virtual ID3D11GeometryShader *GetD3D11GeometryShader() const = 0;
    
    virtual ID3D11DomainShader *GetD3D11DomainShader() const = 0;
    
    virtual ID3D11HullShader *GetD3D11HullShader() const = 0;
    
    virtual ID3D11ComputeShader *GetD3D11ComputeShader() const = 0;
};
} // namespace PPGE
