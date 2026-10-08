cmake_minimum_required(VERSION 3.20)

project(GAIAI VERSION 0.1.0 LANGUAGES CXX)

option(GAIA_BUILD_EDITOR "Build the authoring editor executable" ON)
option(GAIA_BUILD_RUNTIME "Build the distributable runtime executable" ON)
option(GAIA_BUILD_TESTS "Build automated tests" ON)

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
