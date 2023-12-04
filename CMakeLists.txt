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

set(universal_compiler_flags "-fsanitize=undefined -fsanitize-trap -pthread -fuse-ld=lld -fno-omit-frame-pointer -fstack-protector-all -fms-extensions -fdiagnostics-color=always -gdwarf-2 -O0")
#set(universal_compiler_flags "-pthread -fuse-ld=lld -fno-omit-frame-pointer -fstack-protector-all -fms-extensions -fdiagnostics-color=always -gdwarf-2 -O0")
set(universal_cxxflags "${CMAKE_CXX_FLAGS} ${universal_compiler_flags}")
set(universal_cflags   "${CMAKE_C_FLAGS} ${universal_compiler_flags}")
set(warn_flags "-Wall -Wextra -Werror")

set(CMAKE_CXX_FLAGS "${universal_cflags} ${warn_flags}")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${universal_cflags} ${warn_flags}")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_RC_COMPILER_INIT windres)
ENABLE_LANGUAGE(RC)
  SET(CMAKE_RC_COMPILE_OBJECT
      "<CMAKE_RC_COMPILER> <FLAGS> <DEFINES> -o <OBJECT> <SOURCE>")

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
