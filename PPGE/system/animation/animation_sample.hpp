#pragma once

#include "joint_pose.hpp"

#include <memory>
#include <string>
#include <vector>

namespace PPGE
{
namespace AnimationSystem
{
/**
 * Animation sample for time t that has the joint poses on that time.
 */
class AnimationSample
{
  public:
    AnimationSample(size_t jointCount)
    {
        _jointPoses.resize(jointCount);
    }
    AnimationSample(std::vector<std::shared_ptr<JointPose>> poses) : _jointPoses(std::move(poses)){};
    void setJointPoses(std::vector<std::shared_ptr<JointPose>> &&jointPoses)
    {
        _jointPoses = std::move(jointPoses);
    }
    [[nodiscard]] JointPose getPose(int i);

  private:
    // skeleton joints
    std::vector<std::shared_ptr<JointPose>> _jointPoses;
};
} // namespace AnimationSystem
} // namespace PPGE
