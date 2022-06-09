#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "ecs/components/mesh_component.h"
#include "ecs/entity.h"
#include "renderer/renderer_types.h"
#include "rhi/buffer.h"
#include "system/logger_system.h"
#include "system/renderer_system.h"

namespace PPGE
{
class Entity;

class GeometryHelper
{
  public:
    template <typename VertexBufferType>
    static std::shared_ptr<PPGEBuffer> CreateVertexBuffer(const std::vector<VertexBufferType> &vertices)
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
    static std::shared_ptr<PPGEBuffer> CreateIndexBuffer(const std::vector<IndexBufferType> &indices)
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

    template <typename VertexBufferType, typename IndexBufferType>
    static void CreateMeshFilter(Entity &entity, const std::vector<VertexBufferType> &vertices,
                                 const std::vector<IndexBufferType> &indices)
    {
        std::shared_ptr<PPGEBuffer> vb = CreateVertexBuffer(vertices);

        std::shared_ptr<PPGEBuffer> ib = CreateIndexBuffer(indices);

        auto &mesh_filter = entity.AddComponent<MeshFilterComponent>();
        mesh_filter.vertex_buffer = vb;
        mesh_filter.index_buffer = ib;
        mesh_filter.num_indices = indices.size();
    }
};

class GeometryPrimitives
{
  public:
    static void CreateTriangle(Entity &entity);

    static void CreatePlaneMesh(Entity &entity);

    static void CreateCubeMesh(Entity &entity);

    static void CreateCapsuleMesh(Entity &entity);

    static void CreateSphereMesh(Entity &entity);
};
} // namespace PPGE
