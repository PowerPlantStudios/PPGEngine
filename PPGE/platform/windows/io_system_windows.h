#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/os_helper.h"
#include "system/logger_system.h"
#include "system/io_system.h"

namespace PPGE
{

class PPGE_API IOSystemWindows final : public IOSystem
{
  public:
    IOSystemWindows()
    {
    }
    ~IOSystemWindows()
    {
    }

    void StartUp(const IOSystemProps &props) override final
    {
    }
    
    void ShutDown() override final
    {
    }

    std::unique_ptr<FileSystemObserver> GetFileSystemObserver() override final;
};
} // namespace PPGE