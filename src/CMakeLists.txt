# Setup
cmake_minimum_required   (VERSION 3.20)
project                  (wolf3d-clone)
set                      (CMAKE_CXX_STANDARD 17)

# Specify cpp files
file                     (GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/*.cpp")

# Add cpp files into executable
add_executable           (wolf3d-clone ${SRC_FILES})

# Find and link SDL2 to executable
find_package             (SDL2 REQUIRED)
include_directories      (${SDL2_INCLUDE_DIRS})
target_link_libraries    (wolf3d-clone PRIVATE SDL2)

# Supress all compiler warnings
target_compile_options   (wolf3d-clone PRIVATE -w)
