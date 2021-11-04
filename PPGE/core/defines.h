#pragma once

#if defined(PPGE_PLATFORM_WIN)
#if defined(PPGE_DLL_EXPORT)
#define PPGE_API __declspec(dllexport)
#elif defined(PPGE_DLL_IMPORT)
#define PPGE_API __declspec(dllimport)
#else
#define PPGE_API
#endif
#elif defined(PPGE_PLATFORM_APPLE) || defined(PPGE_PLATFORM_LINUX)
#if defined(PPGE_DLL_BUILD)
#define PPGE_API __attribute__((visibility("default")))
#elif defined(PPGE_DLL_IMPORT)
#define PPGE_API __attribute__((weak_import))
#else
#define PPGE_API
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

#if defined(PPGE_PLATFORM_WIN)
#define PPGE_RELEASE_COM(x)                                                                                            \
    {                                                                                                                  \
        if (x)                                                                                                         \
        {                                                                                                              \
            x->Release();                                                                                              \
            x = 0;                                                                                                     \
        }                                                                                                              \
    }
#endif

#if defined(PPGE_DEBUG)
#if defined(PPGE_PLATFORM_WIN)
#define PPGE_BREAKPOINT() __debugbreak()
#elif defined(PPGE_PLATFORM_APPLE)
#define PPGE_BREAKPOINT() __builtin_trap()
#elif defined(PPGE_PLATFORM_LINUX)
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
#if defined(PPGE_PLATFORM_WIN)
#define PPGE_HR(x)                                                                                                     \
    {                                                                                                                  \
        HRESULT __hr = (x);                                                                                            \
        if (FAILED(__hr))                                                                                              \
        {                                                                                                              \
            PPGE_BREAKPOINT();                                                                                         \
        }                                                                                                              \
    }
#endif
#else
#define PPGE_BREAKPOINT()
#define PPGE_ASSERT(exp, ...)
#define APP_ASSERT(exp, ...)
#define PPGE_HR(x) (x)
#endif