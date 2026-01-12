# ==============================================================================
# Project Configuration
# ==============================================================================
cmake_minimum_required (VERSION 3.8)

# Set project name
project ("Clique based")

# [FIX] Giúp IDE (VS Code/CLion) nhận diện đường dẫn để Go to Definition
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++17 standard
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# ==============================================================================
# Include Directories
# ==============================================================================
# [CHECK LẠI] Hãy chắc chắn folder của bạn đúng là: Project/src/include 
# Nếu cấu trúc là Project/include thì sửa dòng dưới thành "${CMAKE_SOURCE_DIR}/include"
include_directories ("${CMAKE_SOURCE_DIR}/src/include")

# ==============================================================================
# Source Files
# ==============================================================================
# Find all .cpp files
#file(GLOB SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/src/*.cpp")
file(GLOB SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/src/*.cpp")

# ==============================================================================
# Build Target - Main
# ==============================================================================
add_executable (main ${SOURCE_FILES})

# ======================================================================
# Post-build: Copy configs
# ======================================================================
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/config"
        "$<TARGET_FILE_DIR:main>/config"
    COMMENT "Copying config folder..."
)

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:main>/data"
    COMMENT "Copying data folder..."
)