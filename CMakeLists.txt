cmake_minimum_required(VERSION 3.20)
project(my_containers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(my_containers INTERFACE)
target_include_directories(my_containers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(example src/main.cpp)
target_link_libraries(example PRIVATE my_containers)

add_subdirectory(tests)
