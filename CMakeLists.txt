cmake_minimum_required(VERSION 3.14)
project(HashTableProject C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

find_package(GTest REQUIRED)

add_library(hash_table_lib src/hash_function.c)
target_include_directories(hash_table_lib PUBLIC include)

add_executable(hash_app src/main.c)
target_link_libraries(hash_app hash_table_lib)

enable_testing()
add_executable(hash_tests tests/test_hash.cpp)

target_link_libraries(hash_tests 
    hash_table_lib 
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hash_tests)