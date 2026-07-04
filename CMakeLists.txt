cmake_minimum_required(VERSION 3.24...3.30)

# 1. Project definitions matching your repository name
project(game-badge
        VERSION 1.0.0
        DESCRIPTION "OBS Studio Game Badge Plugin"
        HOMEPAGE_URL "https://github.com/Reedy-Ropram/game-badge"
)

# 2. Include the official template configuration helpers 
include(cmake/common/buildspec_common.cmake)
include(cmake/common/plugin_common.cmake)

# 3. Create the plugin library module using your new src/ directory
add_library(${CMAKE_PROJECT_NAME} MODULE src/game_badge_plugin.cpp)

# 4. Link against core OBS libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::libobs)

# 5. Define where your local UI asset files live
define_ffi_plugin(${CMAKE_PROJECT_NAME})