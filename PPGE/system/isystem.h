#pragma once
#include "PPGEpch.h"

#include "core/defines.h"

namespace PPGE
{
template <typename ISystemProps> class PPGE_API ISystem
{
  public:
    virtual void StartUp(const ISystemProps &props) = 0;
    virtual void ShutDown() = 0;
};
} // namespace PPGE