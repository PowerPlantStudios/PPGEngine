#include "vertex_layout.h"

namespace PPGE
{
inline uint16_t GetAttributeTypeSize(VertexLayout::Type type)
{
    switch (type)
    {
    case PPGE::VertexLayout::Type::Uint8:
        return 1;
    case PPGE::VertexLayout::Type::Int16:
        return 2;
    case PPGE::VertexLayout::Type::Float:
        return 4;
    }

    PPGE_ASSERT(false, "Unknown Vertex Attribute Type!");
    return 0;
}

VertexLayout::VertexLayout(std::initializer_list<Element> elements) : m_elements(elements)
{
    uint16_t offset = 0;
    uint16_t element_size = 0;
    for (auto &element : m_elements)
    {
        element.m_offset = offset;
        element_size = GetAttributeTypeSize(element.m_type) * element.m_num;
        offset += element_size;
    }
    m_stride = element_size;
}
} // namespace PPGE