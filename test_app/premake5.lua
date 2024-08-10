project "test_app"
    kind "ConsoleApp"
    language "C"
    cdialect "C11"
    staticruntime "on"

    targetdir (binbase .. "/%{prj.name}")
    objdir (binbase .. "/%{prj.name}/obj")

    files { "src/**.c", "src/**.h", "src/shaders.h" }
    includedirs { "src/", "%{wks.location}/engine1/include" }
    includedirs { "%{wks.location}/engine1/lib/glfw3/include" } -- FIXME: Apps shouldn't depend on glfw includes
    links { "engine1" }

    prebuildcommands { "%{shader_gen} %{prj.location}/assets/shaders %{prj.location}/src/shaders.h" }

    filter "system:linux"
        links { "m" }

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
