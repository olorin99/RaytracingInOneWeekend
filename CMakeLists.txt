cmake_minimum_required(VERSION 3.20)
project(RayTracingInOneWeekend)

set(CMAKE_CXX_STANDARD 23)

add_executable(RayTracingInOneWeekend
        src/main.cpp)

include(FetchContent)

FetchContent_Declare(
        Ende
        GIT_REPOSITORY https://github.com/olorin99/Ende
        GIT_TAG        "master"
)
FetchContent_MakeAvailable(Ende)

target_link_libraries(RayTracingInOneWeekend Ende)