#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/ppge_object.h"
#include "rhi/buffer.h"
#include "rhi/pipeline_state.h"
#include "rhi/sampler.h"
#include "rhi/shader.h"
#include "rhi/texture.h"

namespace PPGE
{
class PPGE_API PPGEDevice : public PPGEObject
{
  public:
    virtual void CreateBuffer(const BufferDesc &desc, const BufferData *data,
                              std::shared_ptr<PPGEBuffer> &buffer_sp) = 0;

    virtual void CreatePipelineState(const GfxPipelineStateCreateDesc &desc,
                                     std::shared_ptr<PPGEPipelineState> &PSO_sp) = 0;

    virtual void CreateShader(const ShaderCreateDesc &desc, std::shared_ptr<PPGEShader> &shader_sp) = 0;

    virtual void CreateTexture(const TextureCreateDesc &desc, std::shared_ptr<PPGETexture> &texture_sp) = 0;

    virtual void CreateSampler(const SamplerDesc &desc, std::shared_ptr<PPGESampler> &sampler_sp) = 0;
};
} // namespace PPGE
