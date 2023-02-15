# Setup
cmake_minimum_required (VERSION 3.20)
project (wolf3d-clone)
set (CMAKE_CXX_STANDARD 17)

# Add .cpp files into executable
file (GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/*.cpp")
if (APPLE)
	file (GLOB_RECURSE ASSETS "../assets/*.csv" "../assets/*.bmp")
	add_executable (wolf3d-clone MACOSX_BUNDLE ${SRC_FILES} ${ASSETS})
	set_source_files_properties(${ASSETS} PROPERTIES
		MACOSX_PACKAGE_LOCATION assets
    )
else()
	add_executable (wolf3d-clone ${SRC_FILES})
endif()

# Create pre-compiled header
file (GLOB_RECURSE UTIL_HEADERS "${CMAKE_SOURCE_DIR}/Utilities/*.h")
target_precompile_headers (wolf3d-clone PRIVATE "Utilities/COMMON_HEADERS.h" ${UTIL_HEADERS})

# Find and link SDL2 to executable
find_package (SDL2 REQUIRED)
include_directories (${SDL2_INCLUDE_DIRS})
target_link_libraries (wolf3d-clone PRIVATE SDL2)

# Suppress all compiler warnings
target_compile_options (wolf3d-clone PRIVATE -w)