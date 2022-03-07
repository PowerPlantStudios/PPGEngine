#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/shader_resource_binding.h"

namespace PPGE
{
class PPGE_API PPGEShaderResourceBindingD3D11 : public PPGEShaderResourceBinding
{
  public:
    typedef struct
    {
        std::vector<ID3D11Buffer *> constant_buffers;
        std::vector<ID3D11ShaderResourceView *> shader_resource_views;
        std::vector<ID3D11SamplerState *> sampler_states;
    } D3D11Bindables;
    virtual const D3D11Bindables &GetD3D11Bindables(ShaderTypeFlags shader_type) = 0;
};
} // namespace PPGE