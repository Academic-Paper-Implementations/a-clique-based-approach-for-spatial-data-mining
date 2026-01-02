# ==============================================================================
# Project Configuration
# ==============================================================================
cmake_minimum_required (VERSION 3.8)

# Set project name
project ("Clique based")

# [FIX] Giúp IDE (VS Code/CLion) nhận diện đường dẫn để Go to Definition
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++17 standard
set (CMAKE_CXX_STANDARD 17)
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
set(TEST_SOURCES
    "${CMAKE_SOURCE_DIR}/src/src/candidate_generation.cpp"
    "${CMAKE_SOURCE_DIR}/src/test/test_candidate_generation.cpp"
)

# ==============================================================================
# Build Target - Main
# ==============================================================================
add_executable (test_generation ${TEST_SOURCES} "src/src/miner.cpp" "src/src/calculate_pi.cpp")

# ======================================================================
# Post-build: Copy configs
# ======================================================================
add_custom_command(TARGET test_generation POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/config"
        "$<TARGET_FILE_DIR:test_generation>/config"
    COMMENT "Copying config folder..."
)

add_custom_command(TARGET test_generation POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:test_generation>/data"
    COMMENT "Copying data folder..."
)