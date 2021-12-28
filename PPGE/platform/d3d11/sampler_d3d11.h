#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/descriptors.h"
#include "system/logger_system.h"

namespace PPGE
{
class RendererSystemD3D11;

class PPGE_API SamplerD3D11
{
  public:
    SamplerD3D11();

    ~SamplerD3D11();

    void Destroy();
    bool Create(const SamplerDesc &desc);
    void Set(ShaderResourceTarget target, uint8_t slot);

  private:
    RendererSystemD3D11 *m_renderer;
    ID3D11SamplerState *m_sampler_state;
};
} // namespace PPGE
