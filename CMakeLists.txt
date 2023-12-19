cmake_minimum_required (VERSION 3.20)
project(unit_test_project LANGUAGES CXX)

file(DOWNLOAD
  https://github.com/doctest/doctest/releases/download/v2.4.11/doctest.h
  ${CMAKE_BINARY_DIR}/doctest/doctest.h
  EXPECTED_MD5 0b7fbd89a158063beecba78eb8400fad)

file(GLOB_RECURSE test_src_files CONFIGURE_DEPENDS "test/*.h" "test/*.cpp")
file(GLOB_RECURSE project_src_file CONFIGURE_DEPENDS "src/*.h")
list(FILTER project_src_file EXCLUDE REGEX "Hal/.*")
add_executable(unit_test_executable ${test_src_files} ${project_src_file})
target_include_directories(unit_test_executable PRIVATE
  ${CMAKE_BINARY_DIR}/doctest
  ${CMAKE_SOURCE_DIR}/src)
target_precompile_headers(unit_test_executable PRIVATE ${CMAKE_SOURCE_DIR}/test/PrecompiledHeader.h)
target_compile_options(unit_test_executable PRIVATE -Wall -Wextra)
