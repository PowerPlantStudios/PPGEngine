#pragma once

#ifdef _PPGE_PLATFORM_WIN
	#ifdef _PPGE_DLL_BUILD
		#define _PPGE_API __declspec(dllexport)
	#else
		#define _PPGE_API __declspec(dllimport)
	#endif

#else
	#error Other Platforms Not Ready Yet
#endif