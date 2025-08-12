cmake_minimum_required(VERSION 3.28)
project(Astralis)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE ASTRALIS_SOURCES
    "Code/**.cpp"
)

add_executable(${PROJECT_NAME} ${ASTRALIS_SOURCES})

find_package(glfw3 REQUIRED)
add_subdirectory("Deps/glad")

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad)
