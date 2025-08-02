cmake_minimum_required(VERSION 3.31)
project(Quokka)

set(CMAKE_CXX_STANDARD 23)
enable_testing()

# Header-only ライブラリ
add_library(QuokkaTimeLib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/time/datetime.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/time/calendar.h
)

target_include_directories(QuokkaTimeLib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# メイン実行ファイル
add_executable(Quokka sandbox/main.cpp)
target_link_libraries(Quokka PRIVATE QuokkaTimeLib)

# GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)
if (MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
FetchContent_MakeAvailable(googletest)

# テストバイナリ
add_executable(QuokkaTests
        test/time/test_datetime.cpp
)

target_link_libraries(QuokkaTests
        PRIVATE
        QuokkaTimeLib
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(QuokkaTests)
