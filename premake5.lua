-- premake5.lua
workspace "Cream"
    configurations { "Debug", "Release", "Distribution"}
    platforms { "Win64" }

  filter "configurations:Debug"
		defines {"CREAM_DEBUG"}
		symbols "on"

	filter "configurations:Release"
    defines {"CREAM_RELEASE"}
		optimize "on"

	filter "configurations:Distribution"
    defines {"CREAM_DISTRIBUTION"}
    optimize "on"
    
  filter { "platforms:Win64" }
    systemversion "latest"
    system "Windows"
    architecture "x86_64"

  filter {}

outputDirectory = "%{cfg.system}%{cfg.buildcfg}-%{cfg.architecture}/%{prj.name}"

project "Cream"
  location "Cream"
  kind "StaticLib"
  language "C++"
  cppdialect "C++17"
  staticruntime "on"

  targetdir ("bin/"..outputDirectory)
  objdir ("bin-int/"..outputDirectory)

  pchheader "creampch.h"
  pchsource "Cream/_src/creampch.cpp"

  files {
		"%{prj.name}/_src/**.h",
		"%{prj.name}/_src/**.cpp"
  }

  includedirs {
    "%{prj.name}/src"
  }

  links {

  }

  filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		runtime "Release"
		optimize "on"

  filter {}

project "Playground"
    location "Playground"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/"..outputDirectory)

    files {
		"%{prj.name}/_src/**.h",
		"%{prj.name}/_src/**.cpp"
    }

    links {
      "Cream"
    }
    
  filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		runtime "Release"
    optimize "on"
    
  filter {}