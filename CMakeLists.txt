cmake_minimum_required(VERSION 3.24...3.30)

project(game-badge
        VERSION 1.0.0
        DESCRIPTION "OBS Studio Game Badge Plugin"
        HOMEPAGE_URL "https://github.com/Reedy-Ropram/game-badge"
)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

# ... (Keep your existing if/elseif/else platform includes and target build logic)

# Add this at the very bottom of your CMakeLists.txt
if(APPLE)
    set(CPACK_GENERATOR "productbuild")
    set(CPACK_PACKAGE_NAME "game-badge")
    set(CPACK_PACKAGE_VERSION "1.0.0")
    # This matches the naming convention expected by package-macos
    set(CPACK_PACKAGE_FILE_NAME "game-badge")
    include(CPack)
endif()