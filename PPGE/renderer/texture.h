#pragma once
#include "PPGEpch.h"

#include "core/defines.h"

namespace PPGE
{
enum class TextureFormat
{
    RGBA8 = 0,
    RGBA8I,
    RGBA8U,
    RGBA8S,
    RGBA16,
    RGBA16I,
    RGBA16U,
    RGBA16F,
    RGBA16S,
    RGBA32,
    RGBA32I,
    RGBA32U,
    RGBA32F,
    RGBA32S,

    D16,
    D24,
    D24S8,
    D32,
    D16F,
    D24F,
    D32F
};

struct TextureDesc
{
    TextureFormat m_format;
};

struct Texture2DDesc : public TextureDesc
{
    size_t m_width;
    size_t m_height;
    void *m_data;
};

struct Texture3DDesc : public Texture2DDesc
{
    size_t m_depth;
};
} // namespace PPGE