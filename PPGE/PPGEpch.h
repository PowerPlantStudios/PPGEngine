#pragma once

// Commonly used standard libraries
#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <stdint.h>

// Platform dependent libraries
#ifdef PPGE_PLATFORM_WIN
#include <Windows.h>
#include <d3d11.h>
#endif

// Commonly used (mostly unchanged) PPGE third party libraries

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
