#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/buffer.h"
#include "rhi/rhi_constants.h"
#include "system/logger_system.h"
#include "system/renderer_system.h"

namespace PPGE
{
class Mesh
{
  public:
    Mesh(std::vector<std::shared_ptr<PPGEBuffer>> vertex_buffers);

    Mesh(std::vector<std::shared_ptr<PPGEBuffer>> vertex_buffers, std::shared_ptr<PPGEBuffer> index_buffer,
         uint64_t num_indices);

    void Bind();

    bool IsIndexBufferBound() const
    {
        return m_index_buffer != nullptr;
    }

    uint64_t GetNumIndices() const
    {
        return m_num_indices;
    }

  private:
    void InitializeVertexBuffers(std::vector<std::shared_ptr<PPGEBuffer>> vertex_buffers);

    std::shared_ptr<PPGEBuffer> m_vertex_buffers[Num_Of_Buffer_Slots];
    uint64_t m_offsets[Num_Of_Buffer_Slots];
    std::shared_ptr<PPGEBuffer> m_index_buffer;
    uint64_t m_num_indices;
};

namespace MeshHelper
{
template <typename VertexBufferType>
std::shared_ptr<PPGEBuffer> CreateVertexBuffer(const std::vector<VertexBufferType> &vertices)
{
    std::shared_ptr<PPGEBuffer> vb;
    {
        BufferDesc vb_desc;
        vb_desc.byte_width = vertices.size() * sizeof(VertexBufferType);
        vb_desc.bind_flags = BindFlags::BIND_VERTEX_BUFFER;
        vb_desc.usage = UsageType::USAGE_IMMUTABLE;
        vb_desc.cpu_access_flags = CPUAccessFlags::CPU_ACCESS_NONE;
        BufferData vb_init_data;
        vb_init_data.data_ptr = &vertices[0];
        vb_init_data.data_size = vertices.size() * sizeof(VertexBufferType);
        RendererSystem::Get().GetDevice()->CreateBuffer(vb_desc, &vb_init_data, vb);
    }

    return vb;
}

template <typename IndexBufferType>
std::shared_ptr<PPGEBuffer> CreateIndexBuffer(const std::vector<IndexBufferType> &indices)
{
    std::shared_ptr<PPGEBuffer> ib;
    {
        BufferDesc ib_desc;
        ib_desc.byte_width = indices.size() * sizeof(IndexBufferType);
        ib_desc.bind_flags = BindFlags::BIND_INDEX_BUFFER;
        ib_desc.usage = UsageType::USAGE_IMMUTABLE;
        ib_desc.cpu_access_flags = CPUAccessFlags::CPU_ACCESS_NONE;
        BufferData ib_init_data;
        ib_init_data.data_ptr = &indices[0];
        ib_init_data.data_size = indices.size() * sizeof(IndexBufferType);
        RendererSystem::Get().GetDevice()->CreateBuffer(ib_desc, &ib_init_data, ib);
    }

    return ib;
}

template <typename VertexBufferType>
std::shared_ptr<Mesh> CreateMesh(const std::vector<std::vector<VertexBufferType>> &vertices)
{
    std::vector<std::shared_ptr<PPGEBuffer>> vbs;
    std::for_each(vertices.cbegin(), vertices.cend(), [&](const auto &v) { vbs.push_back(CreateVertexBuffer(v)); });
    return std::make_shared<Mesh>(vbs);
}

template <typename VertexBufferType, typename IndexBufferType>
std::shared_ptr<Mesh> CreateMesh(const std::vector<std::vector<VertexBufferType>> &vertices,
                                 const std::vector<IndexBufferType> &indices)
{
    std::vector<std::shared_ptr<PPGEBuffer>> vbs;
    std::for_each(vertices.cbegin(), vertices.cend(), [&](const auto &v) { vbs.push_back(CreateVertexBuffer(v)); });
    std::shared_ptr<PPGEBuffer> ib = CreateIndexBuffer(indices);
    return std::make_shared<Mesh>(vbs, ib, indices.size());
}

} // namespace MeshHelper

} // namespace PPGE
