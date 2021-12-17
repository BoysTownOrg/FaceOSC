add_library(boystownorg-faceosc-fake-library ../src/ofApp.cpp)
target_include_directories(
  boystownorg-faceosc-fake-library
  PUBLIC ../src
  PUBLIC fake)
target_compile_features(boystownorg-faceosc-fake-library PUBLIC cxx_std_17)

add_executable(boystownorg-faceosc-test-executable main.cpp)
target_link_libraries(boystownorg-faceosc-test-executable sbash64-testcpplite
                      boystownorg-faceosc-fake-library)
