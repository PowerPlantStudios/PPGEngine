#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/pipeline_state.h"

namespace PPGE
{
struct VertexPosColor
{
    float px, py, pz;
    union {
        struct
        {
            uint8_t r;
            uint8_t b;
            uint8_t g;
            uint8_t a;
        } channel;
        uint32_t color;
    };
};

struct VertexPosNorColor
{
    float px, py, pz;
    float nx, ny, nz;
    union {
        struct
        {
            uint8_t r;
            uint8_t b;
            uint8_t g;
            uint8_t a;
        } channel;
        uint32_t color;
    };
};

struct FullVertex
{
    float px, py, pz;
    float nx, ny, nz;
    float tx, ty, tz;
    float btx, bty, btz;
    union {
        struct
        {
            uint8_t r;
            uint8_t b;
            uint8_t g;
            uint8_t a;
        } channel;
        uint32_t color;
    };
    float u1, v1, u2, v2, u3, v3;
};

struct CbCameraData
{
    Math::Matrix view;
    Math::Matrix proj;
    Math::Matrix viewProj;
    Math::Vector3 cameraPosition;
    float padding;
};
constexpr const char* CbCameraDataResourceName = "CameraDataConstBuffer";
} // namespace PPGE