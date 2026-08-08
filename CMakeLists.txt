cmake_minimum_required(VERSION 3.20)
project(house_flipper_2d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)
if(ANDROID AND CMAKE_ANDROID_ARCH_ABI STREQUAL "armeabi-v7a")
    target_compile_definitions(raylib PRIVATE STBIR_NO_SIMD)
endif()

add_library(house_flipper_game STATIC src/Game.cpp)
target_include_directories(house_flipper_game PUBLIC include)
target_compile_options(house_flipper_game PRIVATE -Wall -Wextra -Wpedantic)

if(ANDROID)
    add_library(house_flipper_android SHARED src/main.cpp)
    target_link_libraries(house_flipper_android PRIVATE house_flipper_game raylib log android EGL GLESv2)
    target_compile_definitions(house_flipper_android PRIVATE PLATFORM_ANDROID)
    target_compile_options(house_flipper_android PRIVATE -Wall -Wextra -Wpedantic)
else()
    add_executable(house_flipper src/main.cpp)
    target_link_libraries(house_flipper PRIVATE house_flipper_game raylib)
    target_compile_options(house_flipper PRIVATE -Wall -Wextra -Wpedantic)
endif()

option(NEON_BUILD_TESTS "Build deterministic model tests" ON)
if(NEON_BUILD_TESTS AND NOT ANDROID)
    enable_testing()
    add_executable(house_flipper_tests tests/GameTests.cpp)
    target_link_libraries(house_flipper_tests PRIVATE house_flipper_game)
    target_compile_options(house_flipper_tests PRIVATE -Wall -Wextra -Wpedantic)
    add_test(NAME house_flipper_tests COMMAND house_flipper_tests)
endif()
