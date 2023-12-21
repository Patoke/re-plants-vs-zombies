################################################################################
# libjpg external project
################################################################################

include(ExternalProject)

# support for windows
if(MSVC)
    set(libjpeg_binary "jpeg-static.lib")
    set(zlib_binary "zlibstaticd.lib")
    set(libpng_binary "libpng16_staticd.lib")
else() # linux
    set(libjpeg_binary "libjpeg.a")
    set(zlib_binary "libzlibstatic.a")
    set(libpng_binary "libpng.a")
endif()

set(libjpeg_dir "${CMAKE_BINARY_DIR}/libjpeg")
ExternalProject_Add(libjpeg-src
    GIT_REPOSITORY      https://github.com/libjpeg-turbo/libjpeg-turbo.git
    GIT_TAG             3.0.1
    SOURCE_DIR          "${libjpeg_dir}"
    BINARY_DIR          "${libjpeg_dir}"
    CMAKE_ARGS          "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}"
                        "-DCMAKE_INSTALL_PREFIX=${libjpeg_dir}/install" # unused, have to specify
                        "-DCMAKE_C_FLAGS=${universal_cflags}"
                        "-DENABLE_SHARED=FALSE"
                        "-DWITH_TURBOJPEG=FALSE"
                        "-Wno-dev"

    INSTALL_COMMAND     ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include/jpeg/;
    COMMAND             ${CMAKE_COMMAND} -E copy ${libjpeg_dir}/jpeglib.h
                                                 ${libjpeg_dir}/jerror.h
                                                 ${libjpeg_dir}/jconfig.h
                                                 ${libjpeg_dir}/jmorecfg.h
                                                 ${libjpeg_dir}/jpegint.h
                                            ${CMAKE_BINARY_DIR}/include/jpeg/
    BUILD_BYPRODUCTS    "${libjpeg_dir}/${libjpeg_binary}"
                        "${CMAKE_BINARY_DIR}/include/jpeg/jpeglib.h"
                        "${CMAKE_BINARY_DIR}/include/jpeg/jerror.h"
    USES_TERMINAL_DOWNLOAD TRUE
    USES_TERMINAL_UPDATE TRUE
    USES_TERMINAL_CONFIGURE TRUE
    USES_TERMINAL_BUILD TRUE
    UPDATE_DISCONNECTED TRUE
)

################################################################################
# zlib external project
################################################################################

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

################################################################################
# libpng external project
################################################################################

set(libpng_dir "${CMAKE_BINARY_DIR}/libpng")
ExternalProject_Add(libpng-src
    GIT_REPOSITORY          https://github.com/glennrp/libpng.git
    GIT_TAG                 v1.6.40
    DEPENDS                 zlib-src
    SOURCE_DIR              "${libpng_dir}"
    CMAKE_ARGS              "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}"
                            "-DCMAKE_INSTALL_PREFIX=${libpng_dir}"
                            "-DCMAKE_C_FLAGS=${universal_cflags}"
                            "-DPNG_HARDWARE_OPTIMIZATIONS=yes"
                            "-DPNG_INTEL_SSE=on"
                            "-DPNG_SHARED=OFF"
                            "-DPNG_EXECUTABLES=OFF"
                            "-DPNG_TESTS=OFF"
                            "-DPNG_BUILD_ZLIB=${zlib_dir}"
                            "-DZLIB_INCLUDE_DIRS=${zlib_dir}"
                            "-DZLIB_LIBRARIES=${zlib_dir}/${zlib_binary}"
                            "-Wno-dev"

    INSTALL_COMMAND         ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include/png/
    COMMAND                 ${CMAKE_COMMAND} -E copy ${libpng_dir}/png.h
                                                     ${libpng_dir}/pngconf.h
                                                     ${libpng_dir}/pnglibconf.h
                                                     ${CMAKE_BINARY_DIR}/include/png/                                            
    BINARY_DIR              ${libpng_dir}
    BUILD_BYPRODUCTS        "${libpng_dir}/${libpng_binary}"
                            "${CMAKE_BINARY_DIR}/include/png/png.h" # Use a custom dir to avoid system headers
    USES_TERMINAL_DOWNLOAD  TRUE
    USES_TERMINAL_UPDATE    TRUE
    USES_TERMINAL_CONFIGURE TRUE
    USES_TERMINAL_BUILD     TRUE
    USES_TERMINAL_INSTALL   TRUE
    UPDATE_DISCONNECTED     TRUE
)

add_library(jpeg STATIC IMPORTED)
add_library(zlib STATIC IMPORTED)
add_library(png STATIC IMPORTED)

add_dependencies(jpeg libjpeg-src)
add_dependencies(zlib zlib-src)
add_dependencies(png libpng-src)

set_target_properties(jpeg
    PROPERTIES IMPORTED_LOCATION "${libjpeg_dir}/${libjpeg_binary}"
)

set_target_properties(zlib
    PROPERTIES IMPORTED_LOCATION "${zlib_dir}/${zlib_binary}"
)

set_target_properties(png
    PROPERTIES IMPORTED_LOCATION "${libpng_dir}/${libpng_binary}"
)

target_link_libraries(${PROJECT_NAME} PRIVATE jpeg zlib png)
add_dependencies(${PROJECT_NAME} libjpeg-src zlib-src libpng-src) # Ensures headers exist

set_property(DIRECTORY PROPERTY ADDITIONAL_CLEAN_FILES
   "${libjpeg_dir}"
   "${zlib_dir}"
   "${libpng_dir}"
   "${CMAKE_BINARY_DIR}/include"
)

target_sources(${PROJECT_NAME} PRIVATE
    "ImageLib.cpp"
    "ImageLib.h"
)
