#include "mesh.h"

#include "rhi/buffer.h"
#include "system/renderer_system.h"

namespace PPGE
{
Mesh::Mesh(std::vector<std::shared_ptr<PPGEBuffer>> vertex_buffers) : m_num_indices(0)
{
    InitializeVertexBuffers(std::move(vertex_buffers));
}

Mesh::Mesh(std::vector<std::shared_ptr<PPGEBuffer>> vertex_buffers, std::shared_ptr<PPGEBuffer> index_buffer,
           uint64_t num_indices)
    : m_index_buffer(std::move(index_buffer)), m_num_indices(num_indices)
{
    InitializeVertexBuffers(std::move(vertex_buffers));
}

void Mesh::Bind()
{
    RendererSystem::Get().GetImmediateContext()->SetVertexBuffers(1, m_vertex_buffers, m_offsets);

    if (m_index_buffer)
        RendererSystem::Get().GetImmediateContext()->SetIndexBuffer(m_index_buffer);
}

void Mesh::InitializeVertexBuffers(std::vector<std::shared_ptr<PPGEBuffer>> vertex_buffers)
{
    PPGE_ASSERT(!(vertex_buffers.size() > Num_Of_Buffer_Slots), "Num of slots are exceeded for vertex buffers.");

    size_t idx = 0;
    for (auto &&vb : vertex_buffers)
    {
        m_offsets[idx] = 0;
        m_vertex_buffers[idx++] = std::move(vb);
    }
}
} // namespace PPGE
