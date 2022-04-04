#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/device_object.h"
#include "rhi/renderer_types.h"

namespace PPGE
{
struct BufferDesc : public DeviceObjectDesc
{
    uint64_t byte_width = 0;
    UsageType usage = UsageType::USAGE_DEFAULT;
    BindFlags bind_flags = BindFlags::BIND_NONE;
    CPUAccessFlags cpu_access_flags = CPUAccessFlags::CPU_ACCESS_NONE;
    uint32_t structure_byte_stride = 0;
};

struct BufferData
{
    const void *data_ptr = nullptr;
    uint64_t data_size = 0;
};

class PPGE_API PPGEBuffer : public PPGEDeviceObject
{
  public:
    virtual const BufferDesc &GetDesc() const = 0;
};
} // namespace PPGE
