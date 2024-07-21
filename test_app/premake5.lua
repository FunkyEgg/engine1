project "test_app"
    kind "ConsoleApp"
    language "C"
    cdialect "C11"
    staticruntime "on"

    targetdir (binbase .. "/%{prj.name}")
    objdir (binbase .. "/%{prj.name}/obj")

    files { "src/**.c", "src/**.h" }
    includedirs { "src/", "%{wks.location}/engine1/include" }

    links { "engine1" }

    prebuildcommands { "%{shader_gen} %{prj.location}/assets/shaders %{prj.location}/src/shaders.h" }

    filter "configurations:debug"
        defines "DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:release"
        defines "RELEASE"
        runtime "Release"
        symbols "off"
        optimize "on"
