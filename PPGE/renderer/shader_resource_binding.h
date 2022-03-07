#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/shader_resource_variable.h"

namespace PPGE
{
class PPGE_API PPGEShaderResourceBinding : public PPGEObject
{
  public:
    virtual PPGEShaderResourceVariable *GetVariableByName(const char *name, ShaderTypeFlags shader_type) = 0;

    virtual PPGEShaderResourceVariable *GetVariableByIndex(uint32_t index, ShaderTypeFlags shader_type) = 0;
};
} // namespace PPGE