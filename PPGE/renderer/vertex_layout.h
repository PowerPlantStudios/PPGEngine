#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"

namespace PPGE
{
class VertexLayout
{
  public:
    enum class Attribute
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

    enum class Type
    {
        Uint8 = 0,
        Int16,
        Float
    };

    struct Element
    {
        Attribute m_attribute;
        Type m_type;
        uint8_t m_num;
        uint16_t m_offset = 0;
    };
    
    VertexLayout() = default;

    VertexLayout(std::initializer_list<Element> elements);

    uint16_t GetStride() const
    {
        return m_stride;
    }

    std::vector<Element>::iterator begin()
    {
        return m_elements.begin();
    }
    std::vector<Element>::iterator end()
    {
        return m_elements.end();
    }
    std::vector<Element>::const_iterator begin() const
    {
        return m_elements.begin();
    }
    std::vector<Element>::const_iterator end() const
    {
        return m_elements.end();
    }

  private:
    std::vector<Element> m_elements;
    uint16_t m_stride = 0;
};
} // namespace PPGE