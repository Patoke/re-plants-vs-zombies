################################################################################
# zlib external project
################################################################################

if(MSVC)
    set(zlib_binary "zlibstaticd.lib")
else() # linux
    set(zlib_binary "libzlibstatic.a")
endif()

set(zlib_dir "${CMAKE_BINARY_DIR}/zlib")
ExternalProject_Add(zlib-src
    GIT_REPOSITORY          https://github.com/madler/zlib.git
    GIT_TAG                 v1.3
    SOURCE_DIR              "${zlib_dir}"
    CMAKE_ARGS              "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}"
                            "-DCMAKE_INSTALL_PREFIX=${zlib_dir}"
                            "-DCMAKE_C_FLAGS=${universal_cflags}"
                            "-Wno-dev"
    BUILD_COMMAND           "${CMAKE_COMMAND}"
                            "--build" "${zlib_dir}"
                            "--target" "zlibstatic"
    INSTALL_COMMAND         ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include/zlib/;
    COMMAND                 ${CMAKE_COMMAND} -E copy ${zlib_dir}/zlib.h
                                                     ${zlib_dir}/zconf.h
                                                ${CMAKE_BINARY_DIR}/include/zlib/

    BINARY_DIR              ${zlib_dir}
    BUILD_BYPRODUCTS        "${zlib_dir}/${zlib_binary}"
                            "${CMAKE_BINARY_DIR}/include/zlib/zlib.h" # Use a custom dir to avoid system headers
    USES_TERMINAL_DOWNLOAD  TRUE
    USES_TERMINAL_UPDATE    TRUE
    USES_TERMINAL_CONFIGURE TRUE
    USES_TERMINAL_BUILD     TRUE
    USES_TERMINAL_INSTALL   TRUE
    UPDATE_DISCONNECTED     TRUE
)

add_library(zlib STATIC IMPORTED)
add_dependencies(zlib zlib-src)
set_target_properties(zlib
    PROPERTIES IMPORTED_LOCATION "${zlib_dir}/${zlib_binary}"
)

target_link_libraries(${PROJECT_NAME} PRIVATE zlib)
add_dependencies(${PROJECT_NAME} zlib-src) # Ensures headers exist

set_property(DIRECTORY PROPERTY ADDITIONAL_CLEAN_FILES
   "${CMAKE_BINARY_DIR}/include"
)

target_sources(${PROJECT_NAME} PRIVATE
    "ImageLib.cpp"
    "ImageLib.h"
)
