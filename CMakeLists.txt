cmake_minimum_required(VERSION 3.10)
project(Octree)

# use C++17 (in case std::fs needed)
set(CMAKE_CXX_STANDARD 17)

# autocomplete with YCM & debug with gdb (needed by valgrind too)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# main
file(GLOB SRC "src/*.cpp")
add_executable(main ${SRC})
target_include_directories(main PRIVATE include)
