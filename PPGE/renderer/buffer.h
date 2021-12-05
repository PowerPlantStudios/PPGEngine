#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"
#include "renderer/vertex_layout.h"

namespace PPGE
{
enum class UsageType
{
    Default = 0,
    Immutable,
    Dynamic,
    Staging
};

enum class CPUFlag
{
    None = 0,
    Read,
    Write
};

struct BufferDesc
{
    void *m_data = nullptr;
    size_t m_size = 0;
    UsageType m_usage = UsageType::Default;
    CPUFlag m_cpu_flags = CPUFlag::None;
};

typedef BufferDesc IndexBufferDesc;

struct VertexBufferDesc : public BufferDesc
{
    VertexLayout m_layout;
};

} // namespace PPGE
