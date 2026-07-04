cmake_minimum_required(VERSION 3.24...3.30)

# 1. Pull in the core template variables
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

# 2. Define the project properties
project(game-badge
        VERSION 1.0.0
        DESCRIPTION "OBS Studio Game Badge Plugin"
        HOMEPAGE_URL "https://github.com/Reedy-Ropram/game-badge"
)

# 3. Explicitly load the platform-specific defaults and helpers that contain the path logic
if(WIN32)
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/windows/defaults.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/windows/helpers.cmake")
elseif(APPLE)
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/macos/defaults.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/macos/helpers.cmake")
else()
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/linux/defaults.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/linux/helpers.cmake")
endif()

# 4. Now that the template's OS helpers are active, find the package
find_package(libobs REQUIRED)

# 5. Build your plugin target module
add_library(${CMAKE_PROJECT_NAME} MODULE)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    src/game_badge_plugin.cpp
)

# 6. Link against the target definition
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
    OBS::libobs
)