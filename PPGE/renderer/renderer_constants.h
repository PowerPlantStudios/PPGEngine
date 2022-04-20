#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/pipeline_state.h"

namespace PPGE
{
inline constexpr InputElementDesc Pos_Color_Layout[] = {{"POSITION", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
                                                        {"COLOR", 0, ElementValueType::ELEMENT_VALUE_UINT32, 1, 0}};

inline constexpr InputElementDesc Pos_Color_Normal_Layout[] = {
    {"POSITION", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
    {"NORMAL", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
    {"COLOR", 0, ElementValueType::ELEMENT_VALUE_UINT32, 1, 0}};

inline constexpr InputElementDesc Textureless_Layout[] = {
    {"POSITION", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
    {"NORMAL", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
    {"TANGENT", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
    {"BITANGENT", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
    {"COLOR", 0, ElementValueType::ELEMENT_VALUE_UINT32, 1, 0}};

inline constexpr InputElementDesc Full_Layout[] = {{"POSITION", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
                                                   {"NORMAL", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
                                                   {"TANGENT", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
                                                   {"BITANGENT", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
                                                   {"COLOR", 0, ElementValueType::ELEMENT_VALUE_UINT32, 1, 0},
                                                   {"TEXTURE", 0, ElementValueType::ELEMENT_VALUE_FLOAT32, 2, 0},
                                                   {"TEXTURE", 1, ElementValueType::ELEMENT_VALUE_FLOAT32, 2, 0},
                                                   {"TEXTURE", 2, ElementValueType::ELEMENT_VALUE_FLOAT32, 2, 0}};

inline constexpr std::array<std::pair<const char *, const InputElementDesc *>, 4> Input_Layouts = {
    std::make_pair("FullLayout", Full_Layout), std::make_pair("TexturelessLayout", Textureless_Layout),
    std::make_pair("PosColorNormalLayout", Pos_Color_Normal_Layout),
    std::make_pair("PosColorLayout", Pos_Color_Layout)};

constexpr const InputElementDesc *GetLayout(const char *lookup_name)
{
    for (auto &layout : Input_Layouts)
    {
        if (std::strcmp(lookup_name, layout.first) == 0)
            return layout.second;
    }
    return nullptr;
}

constexpr const char *PosColorLayout = "PosColorLayout";
constexpr const char *PosColorNormalLayout = "PosColorNormalLayout";
constexpr const char *TexturelessLayout = "TexturelessLayout";
constexpr const char *FullLayout = "FullLayout";
} // namespace PPGE