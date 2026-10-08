cmake_minimum_required(VERSION 3.20)

project(GAIAI VERSION 0.1.0 LANGUAGES CXX)

option(GAIA_BUILD_EDITOR "Build the authoring editor executable" ON)
option(GAIA_BUILD_RUNTIME "Build the distributable runtime executable" ON)
option(GAIA_BUILD_TESTS "Build automated tests" ON)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

add_library(GaiaCore STATIC
    src/core/json.cpp
    src/ecs/world.cpp
    src/scene/scene_loader.cpp
    src/scripting/control_api.cpp)

target_include_directories(GaiaCore PUBLIC include)
target_compile_features(GaiaCore PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(GaiaCore PRIVATE /W4 /WX /permissive- /EHsc)
else()
    target_compile_options(GaiaCore PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

if(GAIA_BUILD_EDITOR)
    add_executable(GaiaEditor apps/editor_main.cpp)
    target_link_libraries(GaiaEditor PRIVATE GaiaCore)
endif()

if(GAIA_BUILD_RUNTIME)
    add_executable(GaiaRuntime apps/runtime_main.cpp)
    target_link_libraries(GaiaRuntime PRIVATE GaiaCore)
    target_compile_definitions(GaiaRuntime PRIVATE GAIA_RUNTIME_BUILD=1)
endif()

install(TARGETS GaiaCore
    ARCHIVE DESTINATION lib)

if(GAIA_BUILD_EDITOR)
    install(TARGETS GaiaEditor
        RUNTIME DESTINATION bin)
endif()

if(GAIA_BUILD_RUNTIME)
    install(TARGETS GaiaRuntime
        RUNTIME DESTINATION bin)
endif()

install(DIRECTORY assets/scenes/
    DESTINATION share/G.A.I.A-I/scenes)
install(FILES README.md
    DESTINATION share/G.A.I.A-I)

if(GAIA_BUILD_TESTS)
    enable_testing()
    add_executable(GaiaTests
        tests/test_main.cpp
        tests/world_tests.cpp
        tests/scene_tests.cpp
        tests/control_api_tests.cpp)
    target_link_libraries(GaiaTests PRIVATE GaiaCore)
    add_test(NAME GaiaTests COMMAND GaiaTests ${CMAKE_CURRENT_SOURCE_DIR}/assets/scenes/feature_coverage.json)
    set_tests_properties(GaiaTests PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

set(CPACK_PACKAGE_NAME "G.A.I.A-I")
set(CPACK_PACKAGE_VENDOR "G.A.I.A-I")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_FILE_NAME "G.A.I.A-I-${PROJECT_VERSION}-windows-x64")
set(CPACK_GENERATOR "ZIP")
include(CPack)
