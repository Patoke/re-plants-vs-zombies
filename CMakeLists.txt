cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

################################################################################
# Target
################################################################################
set(CMAKE_CXX_STANDARD 23)
project(PlantsVsZombies C CXX)

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "List of supported configurations." FORCE
)

# @Patoke todo: probably add MSVC counterparts?
if(MSVC)
    set(universal_compiler_flags "")
else()
    set(universal_compiler_flags "-fuse-ld=lld -Wno-unused-command-line-argument -march=native -fno-omit-frame-pointer -fstack-protector-all -fdiagnostics-color=always -g -pipe")

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(universal_compiler_flags "${universal_compiler_flags} -O3")
    else()
        set(universal_compiler_flags "${universal_compiler_flags} -O0  -fsanitize=address,undefined")
    endif()
endif()
set(universal_cxxflags "${CMAKE_CXX_FLAGS} ${universal_compiler_flags}")
set(universal_cflags   "${CMAKE_C_FLAGS} ${universal_compiler_flags}")

if(MSVC)
    set(warn_flags "")
else()
    set(warn_flags "-Wall -Wextra -Werror")
endif()

set(CMAKE_CXX_FLAGS "${universal_cxxflags} ${warn_flags}")
set(CMAKE_C_FLAGS   "${universal_cflags} ${warn_flags}")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_RC_COMPILER_INIT windres)
ENABLE_LANGUAGE(RC)

add_executable(${PROJECT_NAME})

################################################################################
# Sub-projects
################################################################################
add_subdirectory(src)
