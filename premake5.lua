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
	
project "Simulator"
	location "Simulator"
	kind "ConsoleApp"
	language "C++"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/res/**"
	}
	
	filter "system:Windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"