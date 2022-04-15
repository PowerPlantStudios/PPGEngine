#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/pipeline_state.h"

namespace PPGE
{
struct VertexPosColor
{
    float px, py, pz;
    uint8_t r, g, b, a;
};

struct VertexPosNorColor
{
    float px, py, pz;
    float nx, ny, nz;
    uint8_t r, g, b, a;
};

struct FullVertex
{
    float px, py, pz;
    float nx, ny, nz;
    float tx, ty, tz;
    float btx, bty, btz;
    uint8_t r, g, b, a;
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