workspace "PowerPlantGameEngine"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories of 3rd party projects. 
-- You may refer them in following projects as "%{IncludeDir.KeyValue}".
IncludeDir = {}
IncludeDir["GLFW"] = "Thirdparty/glfw/include"

group "Dependencies" -- Virtual folder that contain 3rd party projects in workspace
	include "Thirdparty/GLFW_premake5.lua"
group "" -- Put the rest of the projects back to the root level of workspace

project "PPGE"
	location "PPGE"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"

	targetdir ("build/bin/" .. outdir .. "/%{prj.name}")
	objdir ("build/obj/" .. outdir .. "/%{prj.nae}")

	pchheader "PPGEpch.h"
	pchsource "PPGE/PPGEpch.cpp"

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	includedirs
	{
		"%{prj.name}",
		"Thirdparty/spdlog/include",
		"%{IncludeDir.GLFW}",
	}

	links 
	{ 
		"GLFW",
		"opengl32.lib",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"PPGE_PLATFORM_WIN",
			"PPGE_DLL_EXPORT"		
		}
		
	filter "configurations:Debug"
		defines "PPGE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PPGE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PPGE_DEBUG"
		optimize "On"

	filter {"system:windows", "configurations.Debug"}
		buildoptions  "/MDd"

	filter {"system:windows", "configurations.Release"}
		buildoptions "/MD"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("build/bin/" .. outdir .. "/%{prj.name}")
	objdir ("build/obj/" .. outdir .. "/%{prj.nae}")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	includedirs
	{
		"PPGE",
		"Thirdparty/spdlog/include",
	}

	links
	{
		"PPGE"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"PPGE_PLATFORM_WIN",
			"PPGE_DLL_IMPORT",
		}
		
		postbuildcommands
		{
			("{COPY} %{wks.location}build/bin/" .. outdir .. "/PPGE/PPGE.dll %{cfg.buildtarget.directory}")
		}

	filter "configurations:Debug"
		defines "PPGE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PPGE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PPGE_DEBUG"
		optimize "On"

	filter {"system:windows", "configurations.Debug"}
		buildoptions "/MDd"

	filter {"system:windows", "configurations.Release"}
		buildoptions "/MD"
