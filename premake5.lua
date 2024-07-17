workspace "engine1"
    architecture "x86_64"
    startproject "test_app"

    configurations { "debug", "release" }

    flags { "MultiProcessorCompile", "NoPCH" }

binbase = "%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}"

group "libs"
    include "engine1/lib/glad"
group ""

group "engine"
    include "engine1/"
group ""

group "examples"
    include "test_app/"
group ""
