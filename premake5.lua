workspace "PowerPlantGameEngine"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "PPGE"
	location "PPGE"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outdir .. "/%{prj.name}")
	objdir ("obj/" .. outdir .. "/%{prj.nae}")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	includedirs
	{
		"Thirdparty/spdlog/include"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"_PPGE_PLATFORM_WIN",
			"_PPGE_DLL_BUILD"		
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

	filter {"system:windows", "configurations.Release"}
		buildoptions "/MT"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outdir .. "/%{prj.name}")
	objdir ("obj/" .. outdir .. "/%{prj.nae}")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	includedirs
	{
		"Thirdparty/spdlog/include",
		"PPGE"
	}

	links
	{
		"PPGE"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"_PPGE_PLATFORM_WIN",
		}
		
		postbuildcommands
		{
			("{COPY} %{wks.location}bin/" .. outdir .. "/PPGE/PPGE.dll %{cfg.buildtarget.directory}")
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

	filter {"system:windows", "configurations.Release"}
		buildoptions "/MT"
