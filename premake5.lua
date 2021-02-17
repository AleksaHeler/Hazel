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

-- Defining specifig projects
project "Hazel"
	location "Hazel"
	kind "SharedLib"
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
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include"
	}

	-- only applies to windows
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL"
			-- we may need "_WINDLL" here
		}

		-- post build step to put .dll where it needs to be
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}
		
	-- only applies to Debug
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		symbols "On"

	-- only applies to Release
	filter "configurations:Release"
		defines "HZ_RELEASE"
		optimize "On"

	-- only applies to debug versions
	filter "configurations:Dist"
		defines "HZ_DIST"
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
		symbols "On"

	-- only applies to Release
	filter "configurations:Release"
		defines "HZ_RELEASE"
		optimize "On"

	-- only applies to debug versions
	filter "configurations:Dist"
		defines "HZ_DIST"
		optimize "On"
	