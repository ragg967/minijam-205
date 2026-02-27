add_rules("mode.debug", "mode.release")
add_requires("raylib 5.5")

target("minijam-205")
    set_kind("binary")
    add_files("src/*.c")
    add_packages("raylib")
    set_rundir("$(projectdir)")
