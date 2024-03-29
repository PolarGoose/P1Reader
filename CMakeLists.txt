cmake_minimum_required (VERSION 3.20)
project(unit_test_project LANGUAGES CXX)

# Download Doctest library
file(DOWNLOAD
  https://github.com/doctest/doctest/releases/download/v2.4.11/doctest.h
  ${CMAKE_BINARY_DIR}/doctest/doctest.h
  EXPECTED_MD5 0b7fbd89a158063beecba78eb8400fad)

# Configure clang-format
file(GLOB_RECURSE all_source_files CONFIGURE_DEPENDS "src/*.h" "src/*.cpp" "test/*.h" "test/*.cpp")
foreach(clangformat_source ${all_source_files})
  get_filename_component(clangformat_source ${clangformat_source} ABSOLUTE)
  list(APPEND clangformat_sources ${clangformat_source})
endforeach()
add_custom_target(clangformat
  COMMAND
    "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/Llvm/x64/bin/clang-format.exe"
    -style=file
    -i
    ${clangformat_sources}
  WORKING_DIRECTORY
    ${CMAKE_SOURCE_DIR}
  COMMENT
    "Run clang-format"
)

# Configure test project

# Create a list of the main project source files without Hal library.
file(GLOB_RECURSE project_src_file CONFIGURE_DEPENDS "src/*.h")
list(FILTER project_src_file EXCLUDE REGEX "Hal/.*")
file(GLOB_RECURSE project_src_file CONFIGURE_DEPENDS "src/Hal/External/*.h")

file(GLOB_RECURSE test_src_files CONFIGURE_DEPENDS "test/*.h" "test/*.cpp")

add_executable(unit_test_executable ${test_src_files} ${project_src_file})
add_dependencies(unit_test_executable clangformat)
target_include_directories(unit_test_executable PRIVATE
  ${CMAKE_BINARY_DIR}/doctest
  ${CMAKE_SOURCE_DIR}/src)
target_precompile_headers(unit_test_executable PRIVATE ${CMAKE_SOURCE_DIR}/test/PrecompiledHeader.h)
target_compile_options(unit_test_executable PRIVATE /std:c++17 /W4 /WX /analyze)
