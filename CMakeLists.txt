cmake_minimum_required(VERSION 3.0.0)
project(RecursiveDescentAnalyser VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(RecursiveDescentAnalyser)

add_executable(RecursiveDescentAnalyser main.cpp)
target_include_directories(RecursiveDescentAnalyser PRIVATE ${PROJECT_BINARY_DIR}/RecursiveDescentAnalyser)
target_link_libraries(RecursiveDescentAnalyser PRIVATE RecursiveDescentAnalyserLib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
