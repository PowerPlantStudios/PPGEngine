#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/isystem.h"
#include "system/logger_system.h"

namespace PPGE
{
enum class OSType
{
    Unknown = 0,
    Windows,
    Linux,
    Mac
};

enum class RendererAPIType
{
    None = 0,
    OpenGL,
    Vulkan,
    D3D11,
    D3D12,
    Metal
};

enum class WindowAPIType
{
    None = 1,
    GLFW,
    Win32
};

inline OSType GetOSType()
{
#if defined(PPGE_PLATFORM_WIN)
    return OSType::Windows;
#elif defined(PPGE_PLATFORM_UNIX)
    return OSType::Linux;
#elif defined(PPGE_PLATFORM_APPLE)
    return OSType::Mac;
#else
    return OSType::Unknown;
#endif
}

inline std::vector<RendererAPIType> GetAvailableRendererAPIs()
{
    auto os_type = GetOSType();
    switch (os_type)
    {
    case OSType::Windows:
        return {RendererAPIType::D3D11, RendererAPIType::D3D12, RendererAPIType::OpenGL, RendererAPIType::Vulkan};
    case OSType::Linux:
        return {RendererAPIType::OpenGL, RendererAPIType::Vulkan};
    case OSType::Mac:
        return {RendererAPIType::Metal};
    case OSType::Unknown:
    default:
        PPGE_ASSERT(false, "Unknown OS Type.")
        return {};
    }
}

inline RendererAPIType GetPreferredRendererAPIType()
{
    auto available_renderer_APIs = GetAvailableRendererAPIs();
    if (!available_renderer_APIs.empty())
    {
        return available_renderer_APIs[0];
    }
    return RendererAPIType::None;
}

inline bool IsRendererAPISupported(RendererAPIType render_api_type)
{
    auto available_renderer_APIs = GetAvailableRendererAPIs();
    if (std::find(available_renderer_APIs.begin(), available_renderer_APIs.end(), render_api_type) !=
        available_renderer_APIs.end())
        return true;
    return false;
}

inline WindowAPIType GetPreferredWindowAPIType()
{
    auto preferred_renderer_api_type = GetPreferredRendererAPIType();
    switch (preferred_renderer_api_type)
    {
    case RendererAPIType::D3D11:
    case RendererAPIType::D3D12:
        return WindowAPIType::Win32;
    case RendererAPIType::OpenGL:
    case RendererAPIType::Vulkan:
    case RendererAPIType::Metal:
        return WindowAPIType::GLFW;
    default:
        PPGE_ASSERT(false, "Unknown Renderer API.")
        return WindowAPIType::None;
    }
}
} // namespace PPGE