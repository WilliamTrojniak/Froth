-- premake5.lua
workspace "Froth"
    configurations { "Debug", "Release", "Distribution"}
    platforms { "Win64" }
    startproject "Playground"

  filter "configurations:Debug"
		defines {"FROTH_DEBUG"}
		symbols "on"

	filter "configurations:Release"
    defines {"FROTH_RELEASE"}
		optimize "on"

	filter "configurations:Distribution"
    defines {"FROTH_DISTRIBUTION"}
    optimize "on"
    
  filter { "platforms:Win64" }
    systemversion "latest"
    system "Windows"
    defines {"FROTH_PLATFORM_WINDOWS"}
    architecture "x86_64"

  filter {}

outputDirectory = "%{cfg.system}%{cfg.buildcfg}-%{cfg.architecture}/%{prj.name}"

group "Dependencies"
  include "Froth/dependencies/Logger"
  include "Froth/dependencies/FMath"
  include "Froth/dependencies/vendor/GLFW"
group ""

project "Froth"
  location "Froth"
  kind "StaticLib"
  language "C++"
  cppdialect "C++17"
  staticruntime "on"

  targetdir ("bin/"..outputDirectory)
  objdir ("bin-int/"..outputDirectory)

  pchheader "frothpch.h"
  pchsource "%{prj.name}/_src/frothpch.cpp"

  files {
		"%{prj.name}/_src/**.h",
		"%{prj.name}/_src/**.cpp"
  }

  includedirs {
    "%{prj.name}/_src",
    "%{prj.name}/dependencies/Logger/include",
    "%{prj.name}/dependencies/FMath/include",
    "%{prj.name}/dependencies/vendor/spdlog/include",
    "%{prj.name}/dependencies/vendor/GLFW/include",
    "%{prj.name}/dependencies/vendor/glm",
    "C:/VulkanSDK/1.2.141.2/Include"
  }

  libdirs {
    "C:/VulkanSDK/1.2.141.2/Lib"
  }

  links {
    "GLFW",
    "FMath",
    "vulkan-1.lib"
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
    objdir ("bin-int/"..outputDirectory)


    files {
		"%{prj.name}/_src/**.h",
		"%{prj.name}/_src/**.cpp"
    }

    includedirs {
      "Froth/_src",
      "Froth/dependencies/Logger/include",
      "Froth/dependencies/FMath/include",
      "Froth/dependencies/vendor/spdlog/include",
      "Froth/dependencies/vendor/GLFW/include",
      "Froth/dependencies/vendor/glm/",
      "C:/VulkanSDK/1.2.141.2/Include"
	  }

    links {
      "Froth"
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