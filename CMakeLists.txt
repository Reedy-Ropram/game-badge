cmake_minimum_required(VERSION 3.16)

project(game-badge VERSION 1.0.0)

find_package(libobs REQUIRED)

add_library(game-badge MODULE game_badge_plugin.cpp)
target_link_libraries(game-badge PRIVATE libobs)

set_target_properties(game-badge PROPERTIES PREFIX "")

# Install layout files cleanly into the plugin's data directory
install(TARGETS game-badge DESTINATION "${OBS_PLUGIN_DESTINATION}")
install(FILES data/badge.html DESTINATION "${OBS_DATA_DESTINATION}/game-badge")