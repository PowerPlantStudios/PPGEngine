#pragma once

// Commonly used standard libraries
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// Platform dependent libraries
#if defined(PPGE_PLATFORM_WIN)
// Windows libraries
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include <Windows.h>
#include <atlbase.h>
#include <d3d11_4.h>
#include <d3d11sdklayers.h>
#elif defined(PPGE_PLATFORM_UNIX)
// Linux libraries
#include <signal.h>
#elif defined(PPGE_PLATFORM_APPLE)
// Mac libraries
#endif

// Commonly used (mostly unchanged) PPGE third party libraries
// ImGui includes
#include <imgui.h>

// Logger includes
// Do not change order of the includes
#include <spdlog/spdlog.h>

#include <spdlog/fmt/ostr.h>

#include <spdlog/sinks/stdout_color_sinks.h>

#include <spdlog/sinks/basic_file_sink.h>
// Math includes
#include <simple_math.h>
namespace PPGE
{
namespace Math = ::DirectX::SimpleMath;
}

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/version.h>
