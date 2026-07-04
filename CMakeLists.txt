cmake_minimum_required(VERSION 3.24...3.30)

project(game-badge
        VERSION 1.0.0
        DESCRIPTION "OBS Studio Game Badge Plugin"
        HOMEPAGE_URL "https://github.com/Reedy-Ropram/game-badge"
)

# 1. Run the core template variable downloader script
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

# 2. Load the platform-specific defaults and helpers to restore dependency paths
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

# 3. Search for the package using the restored template paths
find_package(libobs REQUIRED)

# 4. Build your standard C++ plugin library module
add_library(${CMAKE_PROJECT_NAME} MODULE)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    src/game_badge_plugin.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
    OBS::libobs
)

# 5. Create the exact nested folder structure the packaging script needs
if(WIN32)
    set(PLUGIN_OUTPUT_DIR "release/bin/64bit")
elseif(APPLE)
    set(PLUGIN_OUTPUT_DIR "release/obs-plugins")
else()
    set(PLUGIN_OUTPUT_DIR "release/obs-plugins/64bit")
endif()

install(TARGETS ${CMAKE_PROJECT_NAME}
        DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/${PLUGIN_OUTPUT_DIR}"
)