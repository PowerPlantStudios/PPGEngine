#pragma once
#include "PPGEpch.h"

#include "core/defines.h"

namespace PPGE
{
enum class ShaderType
{
    VS,
    HS,
    TS,
    GS,
    PS
};

struct ShaderDesc
{
    ShaderType m_type;
    const char *m_data;
    size_t m_size;
};
} // namespace PPGE