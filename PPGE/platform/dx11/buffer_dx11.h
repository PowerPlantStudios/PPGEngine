#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/buffer.h"
#include "system/logger_system.h"

namespace PPGE
{
class RendererSystemDX11;

class PPGE_API BufferD3D11
{
  public:
    BufferD3D11();

    virtual ~BufferD3D11();

    virtual void DestroyBuffer();

  protected:
    RendererSystemDX11 *m_renderer;
    ID3D11Buffer *m_buffer;
};

class PPGE_API IndexBufferD3D11 : public BufferD3D11
{
  public:
    using BufferD3D11::BufferD3D11;

    bool CreateBuffer(const IndexBufferDesc &desc);

    void SetBuffer();

  private:
    DXGI_FORMAT m_format = DXGI_FORMAT_R16_UINT;
};

class PPGE_API VertexBufferD3D11 : public BufferD3D11
{
  public:
    using BufferD3D11::BufferD3D11;

    bool CreateBuffer(const VertexBufferDesc &desc);

    void SetBuffer();

  private:
    UINT m_stride = 0;
    UINT m_offset = 0;
};
} // namespace PPGE
