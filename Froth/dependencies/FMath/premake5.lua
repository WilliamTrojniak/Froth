project "FMath"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputDirectory)
    objdir ("bin-int/" .. outputDirectory)

    files {
        "_src/**.h",
        "_src/**.cpp",
        "include/FMath/**.h"
    }

    filter "system:windows"
        systemversion "latest"
        
    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"