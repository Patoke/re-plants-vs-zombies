target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
    LawnApp.cpp
    Resources.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_BINARY_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "SexyAppFramework") # SexyAppFramework

add_subdirectory(SexyAppFramework)
add_subdirectory(Sexy.TodLib)
add_subdirectory(Lawn)
