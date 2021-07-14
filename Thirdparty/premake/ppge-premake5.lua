workspace "PowerPlantGameEngine"
	architecture "x86_64"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	-- https://github.com/premake/premake-core/wiki/Configurations-and-Platforms --
	-- platforms {"Windows", "Mac"} --

outdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories of 3rd party projects. 
-- You may refer them in following projects as "%{IncludeDir.KeyValue}".
IncludeDir = {}
IncludeDir["GLFW"] = "Thirdparty/glfw/include"
IncludeDir["Glad"] = "Thirdparty/glad/include"
IncludeDir["ImGui"] = "Thirdparty/imgui"

group "Dependencies" -- Virtual folder that contain 3rd party projects in workspace
	include "Thirdparty/glfw/premake5.lua"
	include "Thirdparty/glad/premake5.lua"
	include "Thirdparty/imgui/premake5.lua"
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
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PPGE_PLATFORM_WIN",
			"PPGE_DLL_EXPORT",
			"GLFW_INCLUDE_NONE"
		}

		links
		{
			"GLFW",
			"Glad",
			"ImGui",
			"opengl32.lib",
		}

	filter "system:linux"
		links
		{
			"GLFW",
			"Glad",
			"ImGui",
			"opengl32.lib",
		}
	
	filter {"system:windows", "configurations.Debug"}
		buildoptions  "/MDd"

	filter {"system:windows", "configurations.Release"}
		buildoptions "/MD"

	-- MARK: macosx --
	filter "system:macosx"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PPGE_PLATFORM_MAC",
			"PPGE_DLL_BUILD"
		}
		--buildoptions {"-IThirdparty/spdlog/include"}
		sysincludedirs { "Thirdparty/spdlog/include/" }
		xcodebuildsettings = { ["ALWAYS_SEARCH_USER_PATHS"] = "YES" }
		--GLFW needs to link against several OS X library. Here's the compiler invokation to compile this code sample properly.
		-- source: https://gist.github.com/v3n/27e810ac744b076ceeb7
		links
		{
			"Cocoa.framework",
			"OpenGL.framework",
			"CoreVideo.framework",
			"IOKit.framework",
			"Carbon.framework",
			"glfw"
		}

	-- - building make files on mac specifically
	filter { "system:macosx", "action:gmake"}
		toolset "clang"
	-- macosx ends --

	filter "configurations:Debug"
		defines "PPGE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PPGE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PPGE_DEBUG"
		optimize "On"

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

		includedirs
		{
			"Thirdparty/spdlog/include",
			"PPGE"
		}

	filter {"system:windows", "configurations.Debug"}
		buildoptions "/MDd"

	filter {"system:windows", "configurations.Release"}
		buildoptions "/MD"

	-- MARK: macosx
	filter "system:macosx"

		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PPGE_PLATFORM_MAC",
		}
		-- only includedirs is not working
		sysincludedirs { "Thirdparty/spdlog/include/", "PPGE/"}

		--buildoptions {"-IThirdparty/spdlog/include", "-IPPGE"}
		-- TODO: not sure about this. need to be checked
		links { "Cocoa.framework"}
		postbuildcommands
		{
			("{COPY} %{wks.location}/bin/" .. outdir .. "/PPGE/libPPGE.dylib %{cfg.buildtarget.directory}")
		}

	-- building make files on mac specifically
	filter { "system:macosx", "action:gmake"}
		toolset "clang"

	filter "configurations:Debug"
		defines "PPGE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PPGE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PPGE_DEBUG"
		optimize "On"