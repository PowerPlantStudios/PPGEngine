#pragma once
#include "PPGEpch.h"

#include "Core/defines.h"

namespace PPGE
{
template <typename ISystemProps> class PPGE_API ISystem
{
    virtual void StartUp(const ISystemProps &props) = 0;
    virtual void Update() = 0;
    virtual void ShutDown() = 0;
};
} // namespace PPGE