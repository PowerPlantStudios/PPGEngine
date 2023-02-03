#pragma once

#include "PPGEpch.h"
#include "joint_pose.hpp"
#include "skeleton.hpp"
#include <memory>
#include <simd/simd.h>

#include <unordered_map>

namespace PPGE
{
namespace AnimationSystem
{
class SkeletonPose
{
  public:
    SkeletonPose() : _pSkeleton(std::make_shared<Skeleton>())
    {
    }

    SkeletonPose(std::shared_ptr<Skeleton> pSkeleton);
    void addJointName(std::string name, size_t id) noexcept
    {
        _jointNameToIdMap[name] = id;
    }
    [[nodiscard]] int getJointId(const std::string &name)
    {
        auto x = _jointNameToIdMap.find(name);
        if (x == _jointNameToIdMap.end())
            return -1;

        return (int)x->second;
    }

    void initGlobalPoses()
    {
#if !defined(PPGE_PLATFORM_APPLE)
        _globalPoses.resize(_pSkeleton->jointCount(), Math::Matrix::Identity);
#else
        _globalPoses.resize(_pSkeleton->jointCount(), Math::Identity());
#endif
    }

    std::shared_ptr<Skeleton> getSkeleton()
    {
        return _pSkeleton;
    }

  private:
    std::shared_ptr<Skeleton> _pSkeleton;
    std::unordered_map<std::string, size_t> _jointNameToIdMap{};
    std::vector<JointPose> _localPoses;
    std::vector<simd::float4x4> _globalPoses;
};

} // namespace AnimationSystem

} // namespace PPGE
