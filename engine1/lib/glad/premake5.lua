project "glad"
    kind "StaticLib"
    pic "on"
    language "C"
    staticruntime "on"

    targetdir (binbase .. "/%{prj.name}")
    objdir (binbase .. "/%{prj.name}/obj")

    files {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }
    includedirs { "include/" }

    filter "configurations:debug"
        runtime "Debug"
        symbols "on"
        optimize "off"

    filter "configurations:release"
        runtime "Release"
        symbols "off"
        optimize "on"
