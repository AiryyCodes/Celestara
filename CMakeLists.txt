cmake_minimum_required(VERSION 3.28)
project(Celestara)

file(GLOB_RECURSE CELESTARA_SOURCES
    "Code/**.cpp"
)

add_executable(${PROJECT_NAME} ${CELESTARA_SOURCES})
