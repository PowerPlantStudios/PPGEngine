#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/descriptors.h"
#include "system/logger_system.h"

namespace PPGE
{
class RendererSystemDX11;

class PPGE_API BufferD3D11
{
  public:
    BufferD3D11();

    ~BufferD3D11();

    void Destroy();
    bool Create(const BufferDesc &desc);
    void Update(const Subresource &res);
    void Set(UniformDesc::Target target, uint8_t slot);

    inline UINT GetBufferSize() const
    {
        return m_size;
    }

  private:
    RendererSystemDX11 *m_renderer;
    ID3D11Buffer *m_buffer;
    UINT m_size = 0;
};

class PPGE_API IndexBufferD3D11
{
  public:
    IndexBufferD3D11();

    ~IndexBufferD3D11();

    void Destroy();
    bool Create(const IndexBufferDesc &desc);
    void Set();

    inline UINT GetIndexCount() const
    {
        return m_index_count;
    }

  private:
    RendererSystemDX11 *m_renderer;
    ID3D11Buffer *m_buffer;
    UINT m_index_count = 0;
};

class PPGE_API VertexBufferD3D11
{
  public:
    VertexBufferD3D11();

    ~VertexBufferD3D11();

    void Destroy();
    bool Create(const VertexBufferDesc &desc);
    void Set();

  private:
    RendererSystemDX11 *m_renderer;
    ID3D11Buffer *m_buffer;
    UINT m_stride = 0;
    UINT m_offset = 0;
};
} // namespace PPGE
