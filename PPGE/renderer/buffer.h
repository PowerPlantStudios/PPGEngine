#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"

namespace PPGE
{
enum class UsageType
{
    Default = 0,
    Immutable,
    Dynamic,
    Staging
};

enum class CPUFlag
{
    None = 0,
    Read,
    Write
};

enum class VertexAttribute
{
    Position = 0,
    Normal,
    Tangent,
    Bitangent,
    Color0,
    Color1,
    Color2,
    Color3,
    TexCoord0,
    TexCoord1,
    TexCoord2,
    TexCoord3
};

enum class VertexAttributeType
{
    Uint8 = 0,
    Int16,
    Float
};

inline uint16_t GetAttributeTypeSize(VertexAttributeType type)
{
    switch (type)
    {
    case PPGE::VertexAttributeType::Uint8:
        return 1;
    case PPGE::VertexAttributeType::Int16:
        return 2;
    case PPGE::VertexAttributeType::Float:
        return 4;
    }

    PPGE_ASSERT(false, "Unknown Vertex Attribute Type!");
    return 0;
}

class VertexBufferLayout
{
  public:
    struct VertexElement
    {
        VertexAttribute m_attr;
        VertexAttributeType m_type;
        uint8_t m_num;
        uint16_t m_offset = 0;
    };

    VertexBufferLayout(std::initializer_list<VertexElement> elements) : m_elements(elements)
    {
        uint16_t offset = 0;
        uint16_t element_size = 0;
        for (auto &element : m_elements)
        {
            element.m_offset = offset;

            element_size = GetAttributeTypeSize(element.m_type) * element.m_num;
            offset += element_size;
            m_stride += element_size;
        }
    }

    VertexBufferLayout &Add(VertexAttribute attr, VertexAttributeType type, uint8_t num)
    {
        m_elements.emplace_back(attr, type, num);

        auto &element = m_elements.back();
        element.m_offset = m_stride;

        m_stride += GetAttributeTypeSize(element.m_type) * element.m_num;

        return *this;
    }

    uint16_t GetStride() const
    {
        return m_stride;
    }

    std::vector<VertexElement>::iterator begin()
    {
        return m_elements.begin();
    }
    std::vector<VertexElement>::iterator end()
    {
        return m_elements.end();
    }
    std::vector<VertexElement>::const_iterator begin() const
    {
        return m_elements.begin();
    }
    std::vector<VertexElement>::const_iterator end() const
    {
        return m_elements.end();
    }

  private:
    std::vector<VertexElement> m_elements;
    uint16_t m_stride = 0;
};

struct BufferDesc
{
    void *m_data;
    size_t m_size;
    UsageType m_usage;
    CPUFlag m_cpu_flags;
};

typedef BufferDesc IndexBufferDesc;

struct VertexBufferDesc : public BufferDesc
{
    VertexBufferLayout m_layour;
};

} // namespace PPGE
