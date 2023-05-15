set(LIB_NAME RecursiveDescentAnalyserLib)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRCS)

add_library(${LIB_NAME} STATIC ${SRCS})
target_precompile_headers(${LIB_NAME} PRIVATE pch.h)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)