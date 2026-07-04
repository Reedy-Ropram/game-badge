cmake_minimum_required(VERSION 3.24...3.30)

# 1. Initialize the plugin name and metadata
project(game-badge
        VERSION 1.0.0
        DESCRIPTION "OBS Studio Game Badge Plugin"
        HOMEPAGE_URL "https://github.com/Reedy-Ropram/game-badge"
)

# 2. Fire up the official OBS Template bootloader script
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

# 3. Create the plugin binary module
add_library(${CMAKE_PROJECT_NAME} MODULE)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    src/game_badge_plugin.cpp
)

# 4. Use the correct template target mapping
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
    libobs
)

# 5. Let the template automatically handle installations/packaging layout
setup_plugin_target(${CMAKE_PROJECT_NAME})