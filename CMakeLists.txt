cmake_minimum_required(VERSION 3.24...3.30)

# 1. Fire up the official OBS Template bootloader script FIRST
# This downloads the SDK and updates CMake's search paths in the background
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

# 2. Initialize the plugin name and metadata
project(game-badge
        VERSION 1.0.0
        DESCRIPTION "OBS Studio Game Badge Plugin"
        HOMEPAGE_URL "https://github.com/Reedy-Ropram/game-badge"
)

# 3. Now that the bootstrap paths are loaded, pull in the verified package configurations
find_package(libobs REQUIRED)

# 4. Create your plugin binary module
add_library(${CMAKE_PROJECT_NAME} MODULE)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    src/game_badge_plugin.cpp
)

# 5. Link against the newly found imported target which holds the header folders natively
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
    OBS::libobs
)