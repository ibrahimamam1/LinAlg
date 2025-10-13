cmake_minimum_required(VERSION 3.20)
project(LinalgTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(runTests
  vector_constructor.cpp
)

target_link_libraries(runTests
  GTest::GTest 
  GTest::Main
  pthread
)

add_test(NAME VectorTests COMMAND runTests)



