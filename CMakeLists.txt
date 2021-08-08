cmake_minimum_required (VERSION 3.0)
project (cpp_snippets)

# print out general infos
message("Current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message("Build Type: ${CMAKE_BUILD_TYPE}")

set (CXX_FLAGS_GLOBAL "-O3 -lstdc++ -lm")
set (CMAKE_CXX_STANDARD 14)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(lambda_capture_main lambda_capture_main.cpp)
target_link_libraries(lambda_capture_main PRIVATE Threads::Threads)

add_executable(modulo_main modulo_main.cpp)

# clang-tidy
find_program(
  CLANG_TIDY_EXE
  NAMES "clang-tidy"
  DOC "Path to clang-tidy executable"
  )
if(NOT CLANG_TIDY_EXE)
  message(STATUS "clang-tidy not found.")
else()
  message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
  set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=*,-clang-analyzer-alpha.*,-cppcoreguidelines-pro-type-reinterpret-cast,-cppcoreguidelines-pro-bounds-pointer-arithmetic")
endif()

if(CLANG_TIDY_EXE)
  set_target_properties(
    lambda_capture_main PROPERTIES
    CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
  )
endif()
