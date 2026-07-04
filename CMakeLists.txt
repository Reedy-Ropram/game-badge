cmake_minimum_required(VERSION 3.24...3.30)

project(plugintemplate-for-obs
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

# 3. Handle specific bundle properties for Apple to ensure a .plugin bundle is built
if(APPLE)
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        BUNDLE TRUE
        BUNDLE_EXTENSION "plugin"
    )
endif()

# 4. Standardize the output path directly into release/RelWithDebInfo for all runners
set(PLUGIN_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/release/RelWithDebInfo")

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PLUGIN_OUTPUT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/../.." "${PLUGIN_OUTPUT_DIR}/${CMAKE_PROJECT_NAME}.plugin"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${CMAKE_PROJECT_NAME}>" "${PLUGIN_OUTPUT_DIR}/"
    COMMENT "Deploying compiled binary package directly to target release/RelWithDebInfo layout"
)