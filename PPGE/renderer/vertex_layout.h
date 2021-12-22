#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"

namespace PPGE
{
class VertexLayout
{
  public:
    enum class Attribute : unsigned int
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
        TexCoord3,

        Count
    };

    static const unsigned int AttributeCount = static_cast<unsigned int>(Attribute::Count);

    enum class Type : unsigned int
    {
        Uint8 = 0,
        Int16,
        Float
    };

    union Element {
        struct
        {
            Attribute attribute : 5;
            Type type : 5;
            unsigned int num : 6;
            unsigned int offset : 16;
        } m_fields;
        unsigned int m_code;

        Element(Attribute attribute, Type type, unsigned int num)
        {
            m_fields.attribute = attribute;
            m_fields.type = type;
            m_fields.num = num;
            m_fields.offset = 0U;
        }

        Element(unsigned int code) : m_code(code)
        {
        }
    };

    VertexLayout();
    VertexLayout(std::initializer_list<Element> elements);

    uint16_t GetStride() const
    {
        return m_stride;
    }

    uint64_t GetHashCode() const
    {
        return m_hash_code;
    }

    using ElementsArray = std::array<unsigned int, AttributeCount>;

    ElementsArray::iterator begin()
    {
        return m_elements.begin();
    }

    ElementsArray::iterator end()
    {
        return m_elements.end();
    }

    ElementsArray::const_iterator begin() const
    {
        return m_elements.cbegin();
    }

    ElementsArray::const_iterator end() const
    {
        return m_elements.cend();
    }

  private:
    using ElementsHasher = std::hash<ElementsArray>;
    static ElementsHasher s_hasher;

    ElementsArray m_elements;
    uint16_t m_stride;
    uint64_t m_hash_code;
};
} // namespace PPGE