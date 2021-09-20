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
#endif

// Commonly used (mostly unchanged) PPGE third party libraries
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"