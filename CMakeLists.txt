cmake_minimum_required(VERSION 3.22)
project(project
  VERSION 0.0.1)
enable_testing()

set(CMAKE_CXX_FLAGS_INIT "-Wall -std=c++20")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_SOURCE_DIR "src")
set(PROJECT_SOURCE_LIB_MODULE "${PROJECT_SOURCE_DIR}/ecs")
set(PROJECT_INCLUDE_DIR "include")
set(PROJECT_SPHINX_DIR "docs_sphinx")
set(PROJECT_TEST_DIR "tests")
include_directories("./include/")

file (GLOB SRC_FILES
  "${PROJECT_SOURCE_LIB_MODULE}/*.cpp")

file (GLOB INCLUDE_FILES
  "${PROJECT_INCLUDE_DIR}/*.h*")

file (GLOB TEST_FILES
  "${PROJECT_TEST_DIR}/*.cpp")

find_package(boost)
if ( ${boost_found} )
  include_directories( ${boost_include_dirs} )
endif()

add_executable(project "${PROJECT_SOURCE_DIR}/main.cpp" ${SRC_FILES} ${INCLUDE_FILES})

find_package(Catch2 3 REQUIRED)
# These tests can use the Catch2-provided main
add_executable(tests ${SRC_FILES} ${INCLUDE_FILES})
target_link_libraries(tests PRIVATE Catch2 Catch2::Catch2WithMain)

# These tests need their own main
add_executable(tests-main "${PROJECT_TEST_DIR}/main.cpp" ${TEST_FILES} ${SRC_FILES})
target_link_libraries(tests-main PRIVATE Catch2::Catch2)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

include(CTest)
include(Catch)
catch_discover_tests(tests)

