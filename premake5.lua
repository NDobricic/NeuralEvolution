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
		"%{prj.name}/vendor/spdlog/include"
	}
	
	filter "system:Windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		
project "GUI"
	location "GUI"
	kind "WindowedApp"
	language "C#"
	dotnetframework "4.8"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**",
		"%{prj.name}/res/**"
	}
	
	links
	{
		"System",
		"System.Drawing",
		"System.Windows",
		"System.Windows.Forms",
		"System.ComponentModel"
	}
	
	nuget
	{
		"SkiaSharp:2.80.3",
		"SkiaSharp.Views.Desktop.Common:2.80.3",
		"OpenTK:3.1.0",
		"OpenTK.GLControl:3.1.0",
		"SkiaSharp.Views.WindowsForms:2.80.3"
	}
	
	filter "system:Windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"