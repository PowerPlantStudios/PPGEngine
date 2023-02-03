
namespace PPGE
{
namespace AnimationSystem
{
void AnimationClip::init(size_t animationSampleSize, size_t jointCount)
{
    _animationSamples.resize(animationSampleSize, std::make_shared<AnimationSample>(jointCount));
}
} // namespace AnimationSystem

} // namespace PPGE
