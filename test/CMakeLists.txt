cmake_minimum_required(VERSION 3.20)
project(LinalgTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_COMPILER clang++)
enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(runVectorTests
  vector_constructor.cpp
  vector_operations.cpp
)

target_link_libraries(runVectorTests
  GTest::GTest 
  GTest::Main
  pthread
)
add_test(NAME VectorTests COMMAND runVectorTests)

add_executable(runMatrixTests
  matrix_constructor.cpp
)


target_link_libraries(runMatrixTests
  GTest::GTest 
  GTest::Main
  pthread
)

add_test(NAME MatrixTests COMMAND runMatrixTests)


