cmake_minimum_required(VERSION 3.14)
project(packall_fuzzer)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(fuzztest)

enable_testing()

include(GoogleTest)

fuzztest_setup_fuzzing_flags()
add_executable(
  packall_fuzz
  packall_fuzz.cc
  packall_test.cc
  packall_text_test.cc
  packall_compat_test.cc
  packall_bin_test.cc
)

link_fuzztest(packall_fuzz)
gtest_discover_tests(packall_fuzz)
