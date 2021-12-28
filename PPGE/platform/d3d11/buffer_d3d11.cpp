#include "buffer_d3d11.h"

#include "platform/d3d11/renderer_system_d3d11.h"

namespace PPGE
{
BufferD3D11::BufferD3D11() : m_renderer(RendererSystem::GetRendererSystem<RendererSystemD3D11>()), m_buffer(NULL)
{
}

BufferD3D11::~BufferD3D11()
{
    Destroy();
}

void BufferD3D11::Destroy()
{
    PPGE_RELEASE_COM(m_buffer);
    m_size = 0;
}

bool BufferD3D11::Create(const BufferDesc &desc)
{
    PPGE_ASSERT(m_renderer, "Creating D3D11 resource buffer has failed. RendererSystemD3D11 is inaccessible.");
    Destroy();

    D3D11_BUFFER_DESC bd;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = desc.m_resource.m_size;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;
    bd.StructureByteStride = 0;
    PPGE_HR(m_renderer->m_device->CreateBuffer(&bd, NULL, &m_buffer));

    m_size = desc.m_resource.m_size;

    return m_buffer != NULL;
}

void BufferD3D11::Update(const Subresource &res)
{
    PPGE_ASSERT(m_renderer, "Creating D3D11 resource buffer has failed. RendererSystemD3D11 is inaccessible.");

    D3D11_MAPPED_SUBRESOURCE mapped_resource;
    // Lock the buffer
    PPGE_HR(m_renderer->m_immediate_context->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource));
    memcpy(mapped_resource.pData, res.m_pData, res.m_size);
    // Unlock the buffer
    m_renderer->m_immediate_context->Unmap(m_buffer, 0);
}

void BufferD3D11::Set(ShaderResourceTarget target, uint8_t slot)
{
    PPGE_ASSERT(m_renderer, "Binding D3D11 buffer to the pipeline has failed. RendererSystemD3D11 is inaccessible.");

    UINT number_of_buffer = 1;
    switch (target)
    {
    case ShaderResourceTarget::VS:
        m_renderer->m_immediate_context->VSSetConstantBuffers(slot, number_of_buffer, &m_buffer);
        break;
    case ShaderResourceTarget::HS:
        m_renderer->m_immediate_context->HSSetConstantBuffers(slot, number_of_buffer, &m_buffer);
        break;
    case ShaderResourceTarget::DS:
        m_renderer->m_immediate_context->DSSetConstantBuffers(slot, number_of_buffer, &m_buffer);
        break;
    case ShaderResourceTarget::GS:
        m_renderer->m_immediate_context->GSSetConstantBuffers(slot, number_of_buffer, &m_buffer);
        break;
    case ShaderResourceTarget::PS:
        m_renderer->m_immediate_context->PSSetConstantBuffers(slot, number_of_buffer, &m_buffer);
        break;
    default:
        PPGE_ASSERT(false, "Unknown shader type for setting constant buffer into the pipeline.");
        break;
    }
}

IndexBufferD3D11::IndexBufferD3D11()
    : m_renderer(RendererSystem::GetRendererSystem<RendererSystemD3D11>()), m_buffer(NULL)
{
}

IndexBufferD3D11::~IndexBufferD3D11()
{
    Destroy();
}

void IndexBufferD3D11::Destroy()
{
    PPGE_RELEASE_COM(m_buffer);
    m_index_count = 0;
}

bool IndexBufferD3D11::Create(const IndexBufferDesc &desc)
{
    PPGE_ASSERT(m_renderer, "Creating D3D11 index buffer has failed. RendererSystemD3D11 is inaccessible.");
    Destroy();

    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = desc.m_resource.m_size;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    ibd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = desc.m_resource.m_pData;
    PPGE_HR(m_renderer->m_device->CreateBuffer(&ibd, &iinitData, &m_buffer));

    m_index_count = desc.m_index_cout;

    return m_buffer != NULL;
}

void IndexBufferD3D11::Set()
{
    PPGE_ASSERT(m_renderer,
                "Binding D3D11 index buffer to the pipeline has failed. RendererSystemD3D11 is inaccessible.");

    m_renderer->m_immediate_context->IASetIndexBuffer(m_buffer, DXGI_FORMAT_R16_UINT, 0);
}

VertexBufferD3D11::VertexBufferD3D11()
    : m_renderer(RendererSystem::GetRendererSystem<RendererSystemD3D11>()), m_buffer(NULL)
{
}

VertexBufferD3D11::~VertexBufferD3D11()
{
    Destroy();
}

void VertexBufferD3D11::Destroy()
{
    PPGE_RELEASE_COM(m_buffer);
    m_offset = 0;
    m_stride = 0;
}

bool VertexBufferD3D11::Create(const VertexBufferDesc &desc)
{
    PPGE_ASSERT(m_renderer, "Creating D3D11 vertex buffer has failed. RendererSystemD3D11 is inaccessible.");
    Destroy();

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = desc.m_resource.m_size;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    vbd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = desc.m_resource.m_pData;
    PPGE_HR(m_renderer->m_device->CreateBuffer(&vbd, &vinitData, &m_buffer));

    m_stride = desc.m_layout.GetStride();

    return m_buffer != NULL;
}

void VertexBufferD3D11::Set()
{
    PPGE_ASSERT(m_renderer,
                "Binding D3D11 vertex buffer to the pipeline has failed. RendererSystemD3D11 is inaccessible.");

    m_renderer->m_immediate_context->IASetVertexBuffers(0, 1, &m_buffer, &m_stride, &m_offset);
}
} // namespace PPGE