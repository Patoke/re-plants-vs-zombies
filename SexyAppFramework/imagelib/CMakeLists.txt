################################################################################
# libjpg external project
################################################################################

include(ExternalProject)

set(jpeg_src_dir ${CMAKE_BINARY_DIR}/libjpeg)
set(jpeg_lib_dir ${CMAKE_BINARY_DIR}/libjpeg-build)
ExternalProject_Add(libjpeg-src
    GIT_REPOSITORY      https://github.com/libjpeg-turbo/libjpeg-turbo.git
    GIT_TAG             3.0.1
    SOURCE_DIR          "${jpeg_src_dir}"
    BINARY_DIR          "${jpeg_lib_dir}"
    CMAKE_ARGS          "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}"
                        "-DCMAKE_INSTALL_PREFIX=${jpeg_lib_dir}/install" # unused, have to specify
                        "-DCMAKE_C_FLAGS=${universal_cflags}"
                        "-DENABLE_SHARED=FALSE"
                        "-DWITH_TURBOJPEG=FALSE"
                        "-Wno-dev"

    INSTALL_COMMAND     ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include/jpeg/;
    COMMAND             ${CMAKE_COMMAND} -E copy ${jpeg_src_dir}/jpeglib.h
                                                 ${jpeg_src_dir}/jerror.h
                                                 ${jpeg_lib_dir}/jconfig.h
                                                 ${jpeg_src_dir}/jmorecfg.h
                                                 ${jpeg_src_dir}/jpegint.h
                                            ${CMAKE_BINARY_DIR}/include/jpeg/

    BUILD_BYPRODUCTS    "${jpeg_lib_dir}/libjpeg.a"
                        "${CMAKE_BINARY_DIR}/include/jpeg/jpeglib.h"
                        "${CMAKE_BINARY_DIR}/include/jpeg/jerror.h"
    USES_TERMINAL_DOWNLOAD TRUE
    USES_TERMINAL_UPDATE TRUE
    USES_TERMINAL_CONFIGURE TRUE
    USES_TERMINAL_BUILD TRUE
    UPDATE_DISCONNECTED TRUE
)
add_library(jpeg STATIC IMPORTED)
add_dependencies(jpeg libjpeg-src)
set_target_properties(jpeg
    PROPERTIES IMPORTED_LOCATION ${jpeg_lib_dir}/libjpeg.a
)

################################################################################
# zlib external project
################################################################################

set(zlib_src_dir ${CMAKE_BINARY_DIR}/zlib)
set(zlib_lib_dir ${CMAKE_BINARY_DIR}/zlib-build)
ExternalProject_Add(zlib-src
    GIT_REPOSITORY          https://github.com/madler/zlib.git
    GIT_TAG                 v1.3
    SOURCE_DIR              "${zlib_src_dir}"
    CMAKE_ARGS              "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}"
                            "-DCMAKE_INSTALL_PREFIX=${zlib_lib_dir}"
                            "-DCMAKE_C_FLAGS=${universal_cflags}"
                            "-Wno-dev"
    BUILD_COMMAND           "${CMAKE_COMMAND}"
                            "--build" "${zlib_lib_dir}"
                            "--target" "zlibstatic"
    INSTALL_COMMAND         ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include/zlib/;
    COMMAND                 ${CMAKE_COMMAND} -E copy ${zlib_src_dir}/zlib.h
                                                     ${zlib_lib_dir}/zconf.h
                                                ${CMAKE_BINARY_DIR}/include/zlib/

    BINARY_DIR              ${zlib_lib_dir}
    BUILD_BYPRODUCTS        "${zlib_lib_dir}/libzlibstatic.a"
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
    PROPERTIES IMPORTED_LOCATION ${zlib_lib_dir}/libzlibstatic.a
)

################################################################################
# libpng external project
################################################################################

set(png_src_dir ${CMAKE_BINARY_DIR}/libpng)
set(png_lib_dir ${CMAKE_BINARY_DIR}/libpng-build)
ExternalProject_Add(libpng-src
    GIT_REPOSITORY          https://github.com/glennrp/libpng.git
    GIT_TAG                 v1.6.40
    SOURCE_DIR              "${png_src_dir}"
    CMAKE_ARGS              "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}"
                            "-DCMAKE_INSTALL_PREFIX=${png_lib_dir}"
                            "-DCMAKE_C_FLAGS=${universal_cflags}"
                            "-DPNG_SHARED=OFF"
                            "-DPNG_EXECUTABLES=OFF"
                            "-DPNG_TESTS=OFF"
                            "-Wno-dev"

    INSTALL_COMMAND         ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include/png/
    COMMAND                 ${CMAKE_COMMAND} -E copy ${png_src_dir}/png.h
                                                     ${png_src_dir}/pngconf.h
                                                     ${png_lib_dir}/pnglibconf.h
                                                     ${CMAKE_BINARY_DIR}/include/png/                                            
    BINARY_DIR              ${png_lib_dir}
    BUILD_BYPRODUCTS        "${png_lib_dir}/libpng.a"
                            "${CMAKE_BINARY_DIR}/include/png/png.h" # Use a custom dir to avoid system headers
    USES_TERMINAL_DOWNLOAD  TRUE
    USES_TERMINAL_UPDATE    TRUE
    USES_TERMINAL_CONFIGURE TRUE
    USES_TERMINAL_BUILD     TRUE
    USES_TERMINAL_INSTALL   TRUE
    UPDATE_DISCONNECTED     TRUE
)
add_library(png STATIC IMPORTED)
add_dependencies(png libpng-src)
set_target_properties(png
    PROPERTIES IMPORTED_LOCATION ${png_lib_dir}/libpng.a
)

target_link_libraries(${PROJECT_NAME} PRIVATE jpeg zlib png)
add_dependencies(${PROJECT_NAME} libjpeg-src zlib-src libpng-src) # Ensures headers exist

set_property(DIRECTORY PROPERTY ADDITIONAL_CLEAN_FILES
   "${jpeg_lib_dir}"
   "${zlib_lib_dir}"
   "${png_lib_dir}"
   "${CMAKE_BINARY_DIR}/include"
)

target_sources(${PROJECT_NAME} PRIVATE
    "ImageLib.cpp"
    "ImageLib.h"
)
