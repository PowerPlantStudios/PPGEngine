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
    BufferD3D11() : m_renderer(nullptr), m_buffer(NULL)
    {
    }

    virtual ~BufferD3D11()
    {
    }

    virtual void DestroyBuffer()
    {
        PPGE_RELEASE_COM(m_buffer);
        // TODO : You may optimize this by re-using already 
        // set variable during previous create call;
        m_renderer = nullptr;
    }

  protected:
    RendererSystemDX11 *m_renderer;
    ID3D11Buffer *m_buffer;
};

class PPGE_API IndexBufferD3D11 : public BufferD3D11
{
  public:
    using BufferD3D11::BufferD3D11;

    bool CreateBuffer(const IndexBufferDesc &desc)
    {
        m_renderer = RendererSystem::GetRendererSystem<RendererSystemDX11>();
        if (!m_renderer)
            return false;

        D3D11_BUFFER_DESC ibd;
        ibd.Usage = D3D11_USAGE_IMMUTABLE;
        ibd.ByteWidth = desc.m_size;
        ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ibd.CPUAccessFlags = 0;
        ibd.MiscFlags = 0;
        ibd.StructureByteStride = 0;
        D3D11_SUBRESOURCE_DATA iinitData;
        iinitData.pSysMem = desc.m_data;
        PPGE_HR(m_renderer->m_device->CreateBuffer(&ibd, &iinitData, &m_buffer));

        return true;
    }

    void SetBuffer()
    {
        m_renderer->m_immediate_context->IASetIndexBuffer(m_buffer, m_format, 0);
    }

  private:
    DXGI_FORMAT m_format = DXGI_FORMAT_R16_UINT;
};

class PPGE_API VertexBufferD3D11 : public BufferD3D11
{
  public:
    using BufferD3D11::BufferD3D11;

    bool CreateBuffer(const VertexBufferDesc &desc)
    {
        m_renderer = RendererSystem::GetRendererSystem<RendererSystemDX11>();
        if (!m_renderer)
            return false;

        D3D11_BUFFER_DESC vbd;
        vbd.Usage = D3D11_USAGE_IMMUTABLE;
        vbd.ByteWidth = desc.m_size;
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.CPUAccessFlags = 0;
        vbd.MiscFlags = 0;
        vbd.StructureByteStride = 0;
        D3D11_SUBRESOURCE_DATA vinitData;
        vinitData.pSysMem = desc.m_data;
        PPGE_HR(m_renderer->m_device->CreateBuffer(&vbd, &vinitData, &m_buffer));

        m_stride = desc.m_layout.GetStride();

        return true;
    }

    void SetBuffer()
    {
        m_renderer->m_immediate_context->IASetVertexBuffers(0, 1, &m_buffer, &m_stride, &m_offset);
    }

  private:
    UINT m_stride = 0;
    UINT m_offset = 0;
};
} // namespace PPGE
