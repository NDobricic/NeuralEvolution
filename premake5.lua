workspace "NeuralEvolution"

require "vendor/premake/modules/codeblocks"

	configurations
	{
		"Debug",
		"Release"
	}
	
	platforms
	{
		"x86",
		"x86_64"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

startproject "GUI"

externalproject "GUI"
   location "GUI"
   uuid "0A04B0F6-5EBA-4278-88F3-41FF4932A5C1"
   kind "WindowedApp"
   language "C#"
   dotnetframework "netstandard3.1"
   dependson "Simulator"

project "Simulator"
	location "Simulator"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/res/**"
	}
	
	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/Eigen/Eigen"
	}
	
	postbuildcommands
	{
		"{COPY} ../config.ini ../bin/%{outputdir}"
	}
	
	filter "system:Windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"