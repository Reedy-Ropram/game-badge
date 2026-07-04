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

# 4. Explicit Mac Bundle and Installer Packaging
if(APPLE)
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        BUNDLE TRUE
        BUNDLE_EXTENSION "plugin"
        MACOSX_BUNDLE_BUNDLE_NAME "game-badge"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.game-badge"
    )

    # Use a custom command to force the creation of the .pkg file 
    # and ensure the output directory exists before writing.
    add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/release/$<CONFIG>"
        COMMAND pkgbuild --component "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/../.." 
                         --install-location "/Library/Application Support/obs-studio/plugins" 
                         "${CMAKE_CURRENT_SOURCE_DIR}/release/$<CONFIG>/game-badge.pkg"
        COMMENT "Generating macOS installer package"
    )
endif()

# 5. Cross-Platform Installation Rules
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