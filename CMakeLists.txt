cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0048 NEW)
project(boystownorg-faceosc-test)

include(FetchContent)

FetchContent_Declare(
  testcpplite
  GIT_REPOSITORY https://github.com/sbash64/testcpplite
  GIT_TAG v3.0.0)
FetchContent_MakeAvailable(testcpplite)

add_subdirectory(test)
