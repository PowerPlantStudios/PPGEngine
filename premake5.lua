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

	filter "system:windows"
		includedirs
		{
			"Thirdparty/spdlog/include"
		}

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

	-- MARK: macosx --
	filter "system:macosx"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"_PPGE_PLATFORM_MAC",
			"_PPGE_DLL_BUILD"
		}
		--buildoptions {"-IThirdparty/spdlog/include"}
		sysincludedirs { "Thirdparty/spdlog/include/" }
		xcodebuildsettings = { ["ALWAYS_SEARCH_USER_PATHS"] = "YES" }
		-- - TODO: not sure about this. need to be checked
		links { "Cocoa.framework" }

	-- - building make files on mac specifically
	filter { "system:macosx", "action:gmake"}
		toolset "clang"

	-- macosx ends --

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

		includedirs
		{
			"Thirdparty/spdlog/include",
			"PPGE"
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

	filter { "system:windows", "configurations.Release" }
		buildoptions "/MT"

	-- MARK: macosx
	filter "system:macosx"

		staticruntime "On"
		systemversion "latest"

		defines
		{
			"_PPGE_PLATFORM_MAC",
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