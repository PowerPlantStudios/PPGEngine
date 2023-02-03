#pragma once

#include "PPGEpch.h"

namespace PPGE
{
namespace AnimationSystem
{
class JointPose
{
  public:
    Math::Quaternion _rotation{.0f, .0f, .0f, 1.f};
    Math::Vector3 _translation{.0f, .0f, .0f};
    float _scale{1}; // uniform scale only
};
} // namespace AnimationSystem

} // namespace PPGE
