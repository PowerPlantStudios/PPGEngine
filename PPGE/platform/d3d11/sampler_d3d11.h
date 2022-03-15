#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/sampler.h"

namespace PPGE
{
class PPGE_API PPGESamplerD3D11 : public PPGESampler
{
  public:
    virtual CComPtr<ID3D11SamplerState> GetD3D11SamplerState() const = 0;
};
} // namespace PPGE
