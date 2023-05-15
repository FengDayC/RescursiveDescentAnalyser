cmake_minimum_required(VERSION 3.0.0)
project(RecursiveDescentAnalyser VERSION 0.1.0)

IF(WIN32)
    add_definitions(-DBUILT_IN_WIN32)
ELSEIF(UNIX)
    add_definitions(-DBUILT_IN_UNIX)
ENDIF()

set(LEXICAL_LIB ${CMAKE_CURRENT_SOURCE_DIR}/LexicalAnalyserLib)
set(PARSER_LIB ${CMAKE_CURRENT_SOURCE_DIR}/RecursiveDescentAnalyserLib)
set(PCH ${CMAKE_CURRENT_SOURCE_DIR}/pch.h)

add_subdirectory(LexicalAnalyserLib)
add_subdirectory(RecursiveDescentAnalyserLib)

add_executable(RecursiveDescentAnalyser main.cpp)
target_include_directories(RecursiveDescentAnalyser PRIVATE RecursiveDescentAnalyserLib)
target_link_libraries(RecursiveDescentAnalyser PRIVATE RecursiveDescentAnalyserLib)
