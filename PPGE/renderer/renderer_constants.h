#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/pipeline_state.h"

namespace PPGE
{
template <size_t N, class T> constexpr size_t count_of(T (&)[N])
{
    return N;
}

#define LAYOUT_HELPER(Name, Layout)                                                                                    \
    constexpr std::pair<const InputElementDesc *, size_t> Get##Name##()                                                \
    {                                                                                                                  \
        return std::make_pair(Layout, count_of(Layout));                                                               \
    }

inline constexpr InputElementDesc Pos_Color_Layout[] = {
    {"POSITION", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
    {"COLOR", 0, ElementValueType::ELEMENT_VALUE_UINT32, 1, 0}};
LAYOUT_HELPER(ColorLayout, Pos_Color_Layout)

inline constexpr InputElementDesc Pos_Normal_Color_Layout[] = {
    {"POSITION", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
    {"NORMAL", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
    {"COLOR", 0, ElementValueType::ELEMENT_VALUE_UINT32, 1, 0}};
LAYOUT_HELPER(NormalColorLayout, Pos_Normal_Color_Layout)

inline constexpr InputElementDesc Textured_Layout[] = {
    {"POSITION", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
    {"NORMAL", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
    {"TEXTURE", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 2, 0}};
LAYOUT_HELPER(TexturedLayout, Textured_Layout)

inline constexpr InputElementDesc Standard_Layout[] = {
    {"POSITION", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
    {"NORMAL", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
    {"TANGENT", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
    {"BITANGENT", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
    {"COLOR", 0, ElementValueType::ELEMENT_VALUE_UINT32, 1, 0},
    {"TEXTURE", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 2, 0},
    {"TEXTURE", 1, ElementValueType::ELEMENT_VALUE_FLOAT32, 2, 0},
    {"TEXTURE", 2, ElementValueType::ELEMENT_VALUE_FLOAT32, 2, 0}};
LAYOUT_HELPER(StandardLayout, Standard_Layout)
} // namespace PPGE