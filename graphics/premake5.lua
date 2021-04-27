workspace "Graphics"
	architecture "x32"
	
	startproject "Graphics"
	
	configurations
	{
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


include "vendor/GLFW"
include "vendor/Glad"

project "Graphics"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"res/**.shader",
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"src",
		"vendor",
		"vendor/GLFW/include",
		"vendor/Glad/include",
		"vendor/assimp/include",
		"vendor/glm",
		"vendor/stb_image"
	}

	libdirs{
		"libs"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"assimp-vc141-mtd.lib",
		"opengl32.lib"
	}

	postbuildcommands
	{
		("{COPY} vendor/assimp/assimp-vc141-mtd.dll ../bin/".. outputdir .. "/Graphics")
	}


	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "GLCORE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GLCORE_RELEASE"
		runtime "Release"
		optimize "on"
		
