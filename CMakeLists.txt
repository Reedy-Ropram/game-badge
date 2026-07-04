cmake_minimum_required(VERSION 3.24...3.30)

project(game-badge
        VERSION 1.0.0
        DESCRIPTION "OBS Studio Game Badge Plugin"
        HOMEPAGE_URL "https://github.com/Reedy-Ropram/game-badge"
)

# 1. Load the template bootloader and platform helpers for dependency paths
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

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

find_package(libobs REQUIRED)

# 2. Build the plugin library module
add_library(${CMAKE_PROJECT_NAME} MODULE)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    src/game_badge_plugin.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
    OBS::libobs
)

# 3. Native Template Architecture Integration
if(COMMAND define_obs_plugin_target)
    define_obs_plugin_target(${CMAKE_PROJECT_NAME})
elseif(COMMAND configure_plugin_target)
    configure_plugin_target(${CMAKE_PROJECT_NAME})
endif()

# 4. Correct Cross-Platform Installation Rules for Modules
# By specifying LIBRARY DESTINATION, CMake knows exactly how to export 
# a standalone compiled runtime module object.
if(WIN32 OR LINUX)
    install(TARGETS ${CMAKE_PROJECT_NAME}
            LIBRARY DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/release/$<CONFIG>"
            RUNTIME DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/release/$<CONFIG>"
    )
elseif(APPLE)
    install(TARGETS ${CMAKE_PROJECT_NAME}
            BUNDLE DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/release/$<CONFIG>"
            LIBRARY DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/release/$<CONFIG>"
    )
endif()