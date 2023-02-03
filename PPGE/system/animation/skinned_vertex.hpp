#pragma once

#include <iostream>
#include <simd/simd.h>
#include <vector>

namespace PPGE
{

namespace AnimationSystem
{
class SkinnedVertex
{
  public:
    // Vertex variables
    simd::float3 _position;
    simd::float3 _normal;
    simd::float2 _texCoord;
    // Weights of each bounded joint.
    simd::float4 _jointWeights{0.f, 0.f, 0.f, 0.f};
    // One vertex can bounded up to 4 joint.
    simd::uint4 _jointIndex{0, 0, 0, 0};
};
} // namespace AnimationSystem
} // namespace PPGE
