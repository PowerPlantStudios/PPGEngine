#pragma once
#include "PPGEpch.h"

#include "core/defines.h"

namespace PPGE
{
class PPGE_API SwapChain
{
  public:
    virtual void Swap() = 0;
};
} // namespace PPGE