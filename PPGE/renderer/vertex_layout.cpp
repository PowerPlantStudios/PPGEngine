#include "vertex_layout.h"

namespace PPGE
{
template <class T, size_t N> struct std::hash<std::array<T, N>>
{
    auto operator()(const std::array<T, N> &key) const
    {
        std::hash<T> hasher;
        size_t result = 0;
        for (size_t i = 0; i < N; ++i)
        {
            result = result * 31 + hasher(key[i]); // ??
        }
        return result;
    }
};

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

VertexLayout::ElementsHasher VertexLayout::s_hasher;

VertexLayout::VertexLayout() : m_stride{0}, m_hash_code{0}
{
    m_elements.fill(0);
}

VertexLayout::VertexLayout(std::initializer_list<Element> elements)
{
    m_elements.fill(0);

    uint16_t offset = 0;
    uint16_t element_size = 0;
    unsigned int element_id = 0;
    for (auto element : elements)
    {
        element.m_fields.offset = offset;
        
        element_id = static_cast<std::underlying_type_t<Attribute>>(element.m_fields.attribute);
        m_elements[element_id] = element.m_code;

        element_size = GetAttributeTypeSize(element.m_fields.type) * element.m_fields.num;
        offset += element_size;
    }
    m_stride = offset;

    m_hash_code = s_hasher(m_elements);
}
} // namespace PPGE