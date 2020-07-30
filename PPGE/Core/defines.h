#pragma once

#ifdef PPGE_PLATFORM_WIN
	#ifdef PPGE_DLL_EXPORT
		#define PPGE_API __declspec(dllexport)
	#elif PPGE_DLL_IMPORT
		#define PPGE_API __declspec(dllimport)
	#else
<<<<<<< HEAD
		#define PPGE_API
	#endif

=======
		#define _PPGE_API __declspec(dllimport)
    #endif
#elif _PPGE_PLATFORM_MAC
    #ifdef _PPGE_DLL_BUILD
		#define _PPGE_API __attribute__((visibility("default")))
	#else
		#define _PPGE_API __attribute__((weak_import)) /* NOT SURE if works hope for the best? */
    #endif
>>>>>>> 4cb0b822951fcd761c4c838b1fcc708d1f7b5467
#else
	#error Other Platforms Not Ready Yet
#endif
