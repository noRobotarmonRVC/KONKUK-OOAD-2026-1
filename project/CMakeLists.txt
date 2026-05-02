cmake_minimum_required(VERSION 3.16)

project(RVCProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(USE_REAL_DEVICE "Use real device objects in tests" OFF)

# ============================================================
# GoogleTest
# ============================================================

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# Windows에서 gtest 런타임 충돌 방지
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# ============================================================
# Project sources
# ============================================================

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

# 테스트 빌드에서는 main.cpp 제외
list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*main.cpp$")

# ============================================================
# Main executable
# ============================================================

add_executable(rvc
    src/main.cpp
    ${PROJECT_SOURCES}
)

target_include_directories(rvc PRIVATE
    src/include
)

# ============================================================
# Tests
# ============================================================

enable_testing()

add_executable(DriveControllerTest
    tests/DriveControllerTest.cpp
    ${PROJECT_SOURCES}
)

target_include_directories(DriveControllerTest PRIVATE
    src/include
)

target_link_libraries(DriveControllerTest PRIVATE
    GTest::gtest_main
)

if(USE_REAL_DEVICE)
    target_compile_definitions(DriveControllerTest PRIVATE USE_REAL_DEVICE)
endif()

include(GoogleTest)
gtest_discover_tests(DriveControllerTest)