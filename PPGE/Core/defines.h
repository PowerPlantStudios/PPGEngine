#pragma once

#ifdef _PPGE_PLATFORM_WIN
	#ifdef _PPGE_DLL_BUILD
		#define _PPGE_API __declspec(dllexport)
	#else
		#define _PPGE_API __declspec(dllimport)
    #endif
#elif _PPGE_PLATFORM_MAC
    #ifdef _PPGE_DLL_BUILD
		#define _PPGE_API __attribute__((visibility("default")))
	#else
		#define _PPGE_API __attribute__((weak_import)) /* NOT SURE if works hope for the best? */
    #endif
#else
	#error Other Platforms Not Ready Yet
#endif
