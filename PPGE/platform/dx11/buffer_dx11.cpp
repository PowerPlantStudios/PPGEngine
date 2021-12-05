#include "buffer_dx11.h"

#include "platform/dx11/renderer_system_dx11.h"

namespace PPGE
{
BufferD3D11::BufferD3D11() : m_renderer(RendererSystem::GetRendererSystem<RendererSystemDX11>()), m_buffer(NULL)
{
}

BufferD3D11::~BufferD3D11()
{
    DestroyBuffer();
}

void BufferD3D11::DestroyBuffer()
{
    PPGE_RELEASE_COM(m_buffer);
}

bool IndexBufferD3D11::CreateBuffer(const IndexBufferDesc &desc)
{
    PPGE_ASSERT(m_renderer,
                "Creating D3D11 index buffer has failed. RendererSystemDX11 points to an invalid memory location");

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

    if (m_renderer && m_buffer)
        return true;
    return false;
}

void IndexBufferD3D11::SetBuffer()
{
    m_renderer->m_immediate_context->IASetIndexBuffer(m_buffer, m_format, 0);
}

bool VertexBufferD3D11::CreateBuffer(const VertexBufferDesc &desc)
{
    PPGE_ASSERT(m_renderer,
                "Creating D3D11 vertex buffer has failed. RendererSystemDX11 points to an invalid memory location");

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

    if (m_renderer && m_buffer)
        return true;
    return false;
}

void VertexBufferD3D11::SetBuffer()
{
    m_renderer->m_immediate_context->IASetVertexBuffers(0, 1, &m_buffer, &m_stride, &m_offset);
}

} // namespace PPGE