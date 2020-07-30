#pragma once

// Commonly used standard libraries
#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <stdint.h>

// Platform dependent libraries
#ifdef PPGE_PLATFORM_WIN
	#include <Windows.h>
#endif

// Commonly used (mostly unchanged) PPGE third party libraries
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"