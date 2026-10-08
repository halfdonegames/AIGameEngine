cmake_minimum_required(VERSION 3.25)
project(GAIAEngine VERSION 0.1.0 LANGUAGES CXX)

option(GAIA_BUILD_EDITOR "Build the editor host" ON)
option(GAIA_BUILD_TESTS "Build automated tests" ON)
option(GAIA_ENABLE_VULKAN "Enable Vulkan/NVRHI renderer adapter when dependencies are available" OFF)
option(GAIA_ENABLE_LUA "Enable Lua runtime adapter when Lua is available" OFF)

add_library(gaia-core STATIC
    src/core/json.cpp
    src/ecs/world.cpp
    src/scene/scene_loader.cpp
    src/scripting/control_api.cpp
)
add_library(GAIA::core ALIAS gaia-core)
target_compile_features(gaia-core PUBLIC cxx_std_20)
target_include_directories(gaia-core PUBLIC include)
target_compile_definitions(gaia-core PUBLIC
    $<$<BOOL:${GAIA_ENABLE_VULKAN}>:GAIA_ENABLE_VULKAN=1>
    $<$<BOOL:${GAIA_ENABLE_LUA}>:GAIA_ENABLE_LUA=1>)
if(MSVC)
  target_compile_options(gaia-core PRIVATE /W4 /WX /permissive-)
else()
  target_compile_options(gaia-core PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(gaia-runtime apps/runtime_main.cpp)
target_link_libraries(gaia-runtime PRIVATE GAIA::core)

if(GAIA_BUILD_EDITOR)
  add_executable(gaia-editor apps/editor_main.cpp)
  target_link_libraries(gaia-editor PRIVATE GAIA::core)
endif()

if(GAIA_BUILD_TESTS)
  enable_testing()
  add_executable(gaia-tests tests/test_main.cpp tests/world_tests.cpp tests/scene_tests.cpp tests/control_api_tests.cpp)
  target_link_libraries(gaia-tests PRIVATE GAIA::core)
  target_compile_definitions(gaia-tests PRIVATE GAIA_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
  add_test(NAME gaia-tests COMMAND gaia-tests)
endif()
