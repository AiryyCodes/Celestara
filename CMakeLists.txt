cmake_minimum_required(VERSION 3.28)
project(Celestara)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE ASTRALIS_SOURCES
    "Code/**.cpp"
)

add_executable(${PROJECT_NAME} ${ASTRALIS_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "Code")

add_custom_target(link_target ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/Assets ${CMAKE_BINARY_DIR}/Assets)

find_package(glfw3 REQUIRED)
add_subdirectory("Deps/glad")
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad glm spdlog::spdlog)
