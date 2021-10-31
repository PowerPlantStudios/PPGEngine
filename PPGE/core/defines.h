#pragma once

#ifdef PPGE_PLATFORM_WIN
#ifdef PPGE_DLL_EXPORT
#define PPGE_API __declspec(dllexport)
#elif PPGE_DLL_IMPORT
#define PPGE_API __declspec(dllimport)
#else
#define PPGE_API
#endif
#elif PPGE_PLATFORM_MAC
#ifdef PPGE_DLL_BUILD
#define PPGE_API __attribute__((visibility("default")))
#else
#define PPGE_API __attribute__((weak_import)) /* NOT SURE if works hope for the best? */
#endif
#else
#error Other Platforms Not Ready Yet
#endif

#define PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(function) std::bind(&function, this, std::placeholders::_1)
#define PPGE_BIND_CLASS_METHOD_ARG_COUNT_2(function)                                                                   \
    std::bind(&function, this, std::placeholders::_1, std::placeholders::_2)
#define PPGE_BIND_CLASS_METHOD_ARG_COUNT_3(function)                                                                   \
    std::bind(&function, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define PPGE_BIND_CLASS_METHOD_ARG_COUNT_4(function)                                                                   \
    std::bind(&function, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,                    \
              std::placeholders::_4)
#define SAFE_DELETE(ptr)                                                                                               \
    {                                                                                                                  \
        delete ptr;                                                                                                    \
        ptr = NULL;                                                                                                    \
    }

#ifdef PPGE_DEBUG

#if defined(PPGE_PLATFORM_WIN)
#define PPGE_BREAKPOINT() __debugbreak()
#elif defined(PPGE_PLATFORM_LINUX)
#include <signal.h>
#define PPGE_BREAKPOINT() raise(SIGTRAP)
#endif

#define PPGE_ASSERT(exp, ...)                                                                                          \
    {                                                                                                                  \
        if (!(exp))                                                                                                    \
        {                                                                                                              \
            PPGE_ERROR("Assertion Failed : {0}", __VA_ARGS__);                                                         \
            PPGE_BREAKPOINT();                                                                                         \
        }                                                                                                              \
    }
#define APP_ASSERT(exp, ...)                                                                                           \
    {                                                                                                                  \
        if (!(exp))                                                                                                    \
        {                                                                                                              \
            APP_ERROR("Assertion Failed : {0}", __VA_ARGS__);                                                          \
            PPGE_BREAKPOINT();                                                                                         \
        }                                                                                                              \
    }

#else

#define PPGE_BREAKPOINT()
#define PPGE_ASSERT(exp, ...)

#endif