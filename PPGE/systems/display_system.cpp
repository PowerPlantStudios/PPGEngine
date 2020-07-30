#include "PPGEpch.h"

#include "display_system.h"

#ifdef PPGE_PLATFORM_WIN
	#include "platform/windows/display_system_windows.h"
#endif

namespace PPGE
{
	Shared<DisplaySystem> DisplaySystem::Create(const DisplaySystemProps& props)
	{	
	#ifdef PPGE_PLATFORM_WIN
		return CreateShared<DisplaySystemWindows>(props);
	#else
		//TODO : Other platforms should be added to if else block\

		//TODO : For the else (finally) an assertion macro should log an error message and place platform
		//       specific break point (if assertions are allowed by current configuration.)
		return nullptr;
	#endif
	}

}
