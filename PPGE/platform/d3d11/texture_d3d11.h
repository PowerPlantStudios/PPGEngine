#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/descriptors.h"
#include "system/logger_system.h"

namespace PPGE
{
class RendererSystemD3D11;

class PPGE_API TextureD3D11
{
  public:
    TextureD3D11();

    ~TextureD3D11();

    void Destroy();
    bool Create(const Texture2DDesc &desc);
    void Set();

  private:
    RendererSystemD3D11 *m_renderer;
    ID3D11Texture2D *m_textre;
};
} // namespace PPGE