set(LIB_NAME LexicalAnalyserLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRCS)

add_library(${LIB_NAME} STATIC ${SRCS})
target_precompile_headers(${LIB_NAME} PRIVATE ${PCH})
target_include_directories(${LIB_NAME} PRIVATE ${PROJECT_SOURCE_DIR})