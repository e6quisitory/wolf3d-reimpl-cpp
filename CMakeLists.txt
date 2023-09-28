# Setup
cmake_minimum_required (VERSION 3.20)
project (wolf3d-clone)
set (CMAKE_CXX_STANDARD 20)

# Suppress missing switch cases warnings
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch")
endif()

# Find SDL2
find_package (SDL2 REQUIRED COMPONENTS SDL2)

# Add .cpp files into executable
file (GLOB_RECURSE SRC_FILES
		"${CMAKE_SOURCE_DIR}/src/main.cpp"
		"${CMAKE_SOURCE_DIR}/src/GameEngine/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/DevTools/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/Utilities/*.cpp"
		)

if (APPLE)
	file (GLOB_RECURSE ASSETS "assets/*.csv" "assets/*.bmp")
	add_executable (wolf3d-clone MACOSX_BUNDLE ${SRC_FILES} ${ASSETS})
	set_source_files_properties(${ASSETS} PROPERTIES
		MACOSX_PACKAGE_LOCATION assets
    )
else()
	add_executable (wolf3d-clone ${SRC_FILES})
endif()

# Create pre-compiled header
file (GLOB_RECURSE UTIL_HEADERS "${CMAKE_SOURCE_DIR}/src/Utilities/*.h")
target_precompile_headers (wolf3d-clone PRIVATE "src/Utilities/COMMON_HEADERS.h" ${UTIL_HEADERS})

# Link SDL2
target_link_libraries (wolf3d-clone PRIVATE SDL2::SDL2)
