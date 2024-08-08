project "engine1"
    kind "SharedLib"
    language "C"
    cdialect "C11"
    staticruntime "on"

    targetdir (binbase .. "/%{prj.name}")
    objdir (binbase .. "/%{prj.name}/obj")

    files { "src/**.c", "include/**.h" }
    includedirs {
        "include/",
        "%{wks.location}/engine1/lib/glad/include",
        "%{wks.location}/engine1/lib/glfw3/include"
    }

    links {
        "glfw3",
        "glad"
    }

    filter "configurations:debug"
        defines { "DEBUG", "LOG_LEVEL=4" }
        runtime "Debug"
        symbols "on"
        optimize "off"

    filter "configurations:release"
        defines { "RELEASE", "LOG_LEVEL=2" }
        runtime "Release"
        symbols "off"
        optimize "on"
