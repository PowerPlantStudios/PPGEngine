#pragma once

#ifdef PPGE_PLATFORM_WIN
	#ifdef PPGE_DLL_EXPORT
		#define PPGE_API __declspec(dllexport)
	#elif PPGE_DLL_IMPORT
		#define PPGE_API __declspec(dllimport)
	#else
		#define PPGE_API
	#endif

#else
	#error Other Platforms Not Ready Yet
#endif