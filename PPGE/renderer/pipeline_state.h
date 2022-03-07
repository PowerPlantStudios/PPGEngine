#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/device_object.h"
#include "renderer/shader.h"
#include "renderer/shader_resource_binding.h"
#include "renderer/shader_resource_variable.h"

namespace PPGE
{
struct PipelineStateDesc : public DeviceObjectDesc
{
    PipelineType type = PipelineType::PIPELINE_UNKNOWN;
};

struct GfxPipelineStateDesc : public PipelineStateDesc
{
    GfxPipelineStateDesc()
    {
        type = PipelineType::PIPELINE_GRAPHICS;
    }
};

struct PipelineStateCreateDesc
{
    ShaderResourceCreateDesc *srv = nullptr;
    uint32_t srv_num = 0;
};

struct GfxPipelineStateCreateDesc : public PipelineStateCreateDesc
{
    GfxPipelineStateDesc desc;

    std::shared_ptr<PPGEShader> commited_vs;
    std::shared_ptr<PPGEShader> commited_ps;
    std::shared_ptr<PPGEShader> commited_gs;
    std::shared_ptr<PPGEShader> commited_hs;
    std::shared_ptr<PPGEShader> commited_ds;
    std::shared_ptr<PPGEShader> commited_as;
    std::shared_ptr<PPGEShader> commited_ms;
};

class PPGE_API PPGEPipelineState : public PPGEDeviceObject
{
  public:
    virtual const PipelineStateDesc &GetDesc() const = 0;

    virtual const GfxPipelineStateDesc &GetGraphicsPipelineStateDesc() const = 0;

    virtual PipelineType GetPipelineType() const = 0;

    virtual void CreateShaderResourceBinding(
        std::shared_ptr<PPGEShaderResourceBinding> &pp_shader_resource_binding) = 0;
};
} // namespace PPGE
