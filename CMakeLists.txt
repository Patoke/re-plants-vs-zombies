cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(PlantsVsZombies C CXX)

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "List of supported configurations." FORCE
)

set(CMAKE_CXX_STANDARD 23)

# @Patoke todo: probably add MSVC counterparts?
if(MSVC)
    set(universal_compiler_flags "")
else()
    set(universal_compiler_flags "-fuse-ld=lld -ftime-trace -Wno-unused-command-line-argument -march=native -O3 -fno-omit-frame-pointer -fstack-protector-all -fdiagnostics-color=always -g -pipe")
    #set(universal_compiler_flags "-fuse-ld=lld -fms-extensions -march=native -fdiagnostics-color=always -O3")
endif()
set(universal_cxxflags "${CMAKE_CXX_FLAGS} ${universal_compiler_flags}")
set(universal_cflags   "${CMAKE_C_FLAGS} ${universal_compiler_flags}")

if(MSVC)
    set(warn_flags "")
else()
    set(warn_flags "-Wall -Wextra -Werror -fsanitize=address,undefined")
endif()

set(CMAKE_CXX_FLAGS "${universal_cxxflags} ${warn_flags}")
set(CMAKE_C_FLAGS   "${universal_cflags} ${warn_flags}")
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_RC_COMPILER_INIT windres)
ENABLE_LANGUAGE(RC)

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################
add_subdirectory(SexyAppFramework)
