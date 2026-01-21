cmake_minimum_required(VERSION 3.20)
project(learning-gtest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

# Our beautiful library
add_library(mylib
  lib/algebraClass.cc
)

target_include_directories(mylib PUBLIC lib)

# Consumer app.
add_executable(app app/app.cc)
target_link_libraries(app mylib)

option(BUILD_TESTS "Build unit tests." OFF)
if(BUILD_TESTS)
  enable_testing()
  find_package(GTest REQUIRED)
  include(GoogleTest)
  message(STATUS "Building tests...")

  add_executable(tests 
  tst/tst.cc
  )

  target_link_libraries(tests
    GTest::gmock
    GTest::gtest
    mylib
  )
  gtest_discover_tests(tests)
endif()


option(USE_CLANG_TIDY "Use clang" ON)

if(USE_CLANG_TIDY)
     message(STATUS "Using clang-tidy")
     set(CMAKE_CXX_CLANG_TIDY clang-tidy
     --checks=modernize-*,readability-*,performance-*,-modernize-use-trailing-return-type
     --header-filter=.*
     --warnings-as-errors=*
     )
     add_compile_options(
          -Wunused-variable
          -Wunused-function
          -Wunused-parameter
          -Werror
     )
endif()