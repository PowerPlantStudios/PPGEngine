#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/shader_resource_variable.h"

namespace PPGE
{
class PPGE_API PPGEShaderResourceVariableD3D11 : public PPGEShaderResourceVariable
{
  public:
    virtual ID3D11DeviceChild *GetD3D11Resource() const = 0;
};
} // namespace PPGE
