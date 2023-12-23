cmake_minimum_required(VERSION 3.15)
project(typelist_utils CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(
  CMAKE_EXPORT_COMPILE_COMMANDS
  ON
)

include(FetchContent)

FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.4.0)
FetchContent_MakeAvailable(Catch2)

# Marks Catch2 headers as system so they won't fail when using clang-tidy
get_target_property(CATCH2_IID Catch2 INTERFACE_INCLUDE_DIRECTORIES)
set_target_properties(Catch2 PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${CATCH2_IID}")

add_executable(typelist_utils test/main.cpp test/test_utils.cpp test/test_sort.cpp)
target_include_directories(typelist_utils PRIVATE include)
target_link_libraries(typelist_utils PRIVATE Catch2::Catch2WithMain)

if(ENABLE_COVERAGE)
  target_compile_options(
    typelist_utils
    PRIVATE
      -fprofile-instr-generate
      -fcoverage-mapping
      -mllvm
      -runtime-counter-relocation
  )

  target_link_options(
    typelist_utils
    PRIVATE
    -fprofile-instr-generate
    -fcoverage-mapping
  )
endif()

add_compile_options(
  -Wall
  -Wextra
  -pedantic
  -Werror
)

set(
  CTEST_COVERAGE_COMMAND
  "llvm-cov"
)
enable_testing()
add_test(
  NAME typelist_utils
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  COMMAND
    typelist_utils
)

install(DIRECTORY include/typelist_utils DESTINATION include)
