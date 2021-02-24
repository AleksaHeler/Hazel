-- Defining solution/workspace
workspace "Hazel"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

-- outputdir = Debug-Windows-x64
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/vendor/imgui"

include "Hazel/vendor/GLFW"
include "Hazel/vendor/Glad"
include "Hazel/vendor/imgui"

-- startup project
startproject "Sandbox"

-- Defining specifig projects
project "Hazel"
	location "Hazel"
	kind "SharedLib"
	language "C++"

	-- output directories
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	-- precompiled headers header file
	pchheader "hzpch.h"
	pchsource "Hazel/src/hzpch.cpp"

	-- project files
	files 
	{
		-- recursively search all files
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp"
	}

	-- include directory
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	-- only applies to windows
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE", -- so Glad wont include any OpenGL headers when including GLFW
			"HZ_ENABLE_ASSERTS"
			-- we may need "_WINDLL" here
		}

		-- post build step to put .dll where it needs to be
		postbuildcommands
		{
			-- TODO: try finding out what can be done if the dest folder for copy doesnt exist
			-- Moving postbuildcommands to Sandbox proj is not a good idea because if we only change Hazel,
			-- and only Hazel is built it will not copy the newest dll file to Sandbox
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}
		
	-- only applies to Debug
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug" -- multithreading debug dll
		symbols "On"

	-- only applies to Release
	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release" -- multithreading dll
		optimize "On"

	-- only applies to debug versions
	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release" -- multithreading dll
		optimize "On"
	
	-- enable multithreading on Release on windows
	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"
		
-- Defining project Sandbox
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	-- output directories
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	-- project files
	files 
	{
		-- recursively search all files
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp"
	}

	-- include directory
	includedirs 
	{
		"Hazel/vendor/spdlog/include",
		"Hazel/src"
	}

	-- linking dll
	links
	{
		"Hazel"
	}

	-- only applies to windows
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest" -- or 10.0.18362.0

		defines
		{
			"HZ_PLATFORM_WINDOWS"
			-- we may need "_WINDLL" here
		}
		
	-- only applies to Debug
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"-- multithreading debug dll
		symbols "On"

	-- only applies to Release
	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release" -- multithreading dll
		optimize "On"

	-- only applies to debug versions
	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release" -- multithreading dll
		optimize "On"
	