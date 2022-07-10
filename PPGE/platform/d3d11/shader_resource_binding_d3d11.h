#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/shader_resource_binding.h"

namespace PPGE
{
class PPGE_API PPGEShaderResourceBindingD3D11 : public PPGEShaderResourceBinding
{
  public:
    typedef struct
    {
        // Start slot to range of constant buffers
        std::unordered_map<UINT, std::vector<ID3D11Buffer *>> constant_buffer_ranges;
        // Start slot to range of shader resource views
        std::unordered_map<UINT, std::vector<ID3D11ShaderResourceView *>> shader_resource_view_ranges;
        // Start slot to range of samplers
        std::unordered_map<UINT, std::vector<ID3D11SamplerState *>> sampler_state_ranges;
    } D3D11Bindables;
    virtual const D3D11Bindables &GetD3D11Bindables(ShaderTypeFlags shader_type) = 0;
};
} // namespace PPGE