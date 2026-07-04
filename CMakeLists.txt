cmake_minimum_required(VERSION 3.24...3.30)

# 1. Pull in the template bootloader variables BEFORE defining the project
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

# 2. Initialize project properties
project(game-badge
        VERSION 1.0.0
        DESCRIPTION "OBS Studio Game Badge Plugin"
        HOMEPAGE_URL "https://github.com/Reedy-Ropram/game-badge"
)

# 3. Append the newly generated template helper directories to CMake's search path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules" "${CMAKE_CURRENT_SOURCE_DIR}/cmake/common")

# 4. Now search for the package using the appended module pathways
find_package(libobs REQUIRED)

# 5. Build your plugin target module
add_library(${CMAKE_PROJECT_NAME} MODULE)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    src/game_badge_plugin.cpp
)

# 6. Link using the discovered target definition
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
    OBS::libobs
)