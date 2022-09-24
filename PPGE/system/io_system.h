#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/os_helper.h"
#include "io/file_system_observer.h"
#include "system/isystem.h"
#include "system/logger_system.h"

namespace PPGE
{
struct IOSystemProps
{
};

class PPGE_API IOSystem : public ISystem<IOSystemProps>
{
  public:
    virtual std::unique_ptr<FileSystemObserver> GetFileSystemObserver() = 0;

  public:
    static void Initialize(OSType api);

    inline static void Destroy()
    {
        SAFE_DELETE(s_instance)
    }

    inline static IOSystem &Get()
    {
        return *s_instance;
    }

  private:
    static IOSystem *s_instance;
};
} // namespace PPGE