-- premake5.lua
workspace "Cream"
    configurations { "Debug", "Release", "Distribution"}

outputDirectory = "%{cfg.system}%{cfg.buildcfg}-%{cfg.architecture}/%{prj.name}"

project "Playground"
    location "Playground"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/"..outputDirectory)

    files
	{
		"%{prj.name}/_src/**.h",
		"%{prj.name}/_src/**.cpp"
    }
    
    filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"