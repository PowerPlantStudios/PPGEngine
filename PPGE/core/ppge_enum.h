#pragma once
#include "PPGEpch.h"

#include "core/defines.h"

namespace PPGE
{

namespace Enum
{
template <typename EnumType> constexpr auto ToUnderlyingType(EnumType value)
{
    return static_cast<std::underlying_type_t<EnumType>>(value);
}

template <typename EnumType> std::underlying_type_t<EnumType> &ToUnderlyingTypeRef(EnumType &value)
{
    return reinterpret_cast<std::underlying_type_t<EnumType> &>(value);
}

template <typename EnumType, typename UnderlyingType,
          typename std::enable_if_t<std::is_same_v<std::underlying_type_t<EnumType>, UnderlyingType>> * = nullptr>
constexpr auto ToEnumType(UnderlyingType value)
{
    return static_cast<EnumType>(value);
}

template <typename EnumType, typename UnderlyingType,
          typename std::enable_if_t<std::is_same_v<std::underlying_type_t<EnumType>, UnderlyingType>> * = nullptr>
EnumType &ToEnumTypeRef(UnderlyingType &value)
{
    return reinterpret_cast<EnumType &>(value);
}

template <typename EnumType> constexpr bool ToBoolean(EnumType value)
{
    return static_cast<bool>(ToUnderlyingType(value));
}

} // namespace Enum

#define PPGE_ENUM_OPERATORS(enum_type)                                                                                 \
    inline enum_type &operator|=(enum_type &lhs, enum_type rhs)                                                        \
    {                                                                                                                  \
        return Enum::ToEnumTypeRef<enum_type>(Enum::ToUnderlyingTypeRef(lhs) |= Enum::ToUnderlyingType(rhs));          \
    }                                                                                                                  \
                                                                                                                       \
    inline enum_type &operator&=(enum_type &lhs, enum_type rhs)                                                        \
    {                                                                                                                  \
        return Enum::ToEnumTypeRef<enum_type>(Enum::ToUnderlyingTypeRef(lhs) &= Enum::ToUnderlyingType(rhs));          \
    }                                                                                                                  \
                                                                                                                       \
    inline enum_type &operator^=(enum_type &lhs, enum_type rhs)                                                        \
    {                                                                                                                  \
        return Enum::ToEnumTypeRef<enum_type>(Enum::ToUnderlyingTypeRef(lhs) ^= Enum::ToUnderlyingType(rhs));          \
    }                                                                                                                  \
                                                                                                                       \
    inline constexpr enum_type operator|(enum_type lhs, enum_type rhs)                                                 \
    {                                                                                                                  \
        return Enum::ToEnumType<enum_type>(static_cast<std::underlying_type_t<enum_type>>(                             \
            Enum::ToUnderlyingType(lhs) | Enum::ToUnderlyingType(rhs)));                                               \
    }                                                                                                                  \
                                                                                                                       \
    inline constexpr enum_type operator&(enum_type lhs, enum_type rhs)                                                 \
    {                                                                                                                  \
        return Enum::ToEnumType<enum_type>(static_cast<std::underlying_type_t<enum_type>>(                             \
            Enum::ToUnderlyingType(lhs) & Enum::ToUnderlyingType(rhs)));                                               \
    }                                                                                                                  \
                                                                                                                       \
    inline constexpr enum_type operator^(enum_type lhs, enum_type rhs)                                                 \
    {                                                                                                                  \
        return Enum::ToEnumType<enum_type>(static_cast<std::underlying_type_t<enum_type>>(                             \
            Enum::ToUnderlyingType(lhs) ^ Enum::ToUnderlyingType(rhs)));                                               \
    }                                                                                                                  \
                                                                                                                       \
    inline constexpr enum_type operator~(enum_type val)                                                                \
    {                                                                                                                  \
        return Enum::ToEnumType<enum_type>(                                                                            \
            static_cast<std::underlying_type_t<enum_type>>(~Enum::ToUnderlyingType(val)));                             \
    }

} // namespace PPGE