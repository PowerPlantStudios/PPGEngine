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
    bool Create(const TextureDesc &desc);
    bool Create(const Texture2DDesc &desc);
    bool Create(const Texture3DDesc &desc);
    bool Create(const TextureResurceDesc &desc);
    void Set(ShaderResourceTarget target, uint8_t slot);

  private:
    RendererSystemD3D11 *m_renderer;
    union {
        ID3D11Texture1D *m_texture1D;
        ID3D11Texture2D *m_texture2D;
        ID3D11Texture3D *m_texture3D;
        ID3D11Resource *m_texture;
    };
    ID3D11ShaderResourceView *m_srv;
};
} // namespace PPGE