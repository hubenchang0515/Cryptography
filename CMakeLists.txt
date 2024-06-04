cmake_minimum_required(VERSION 3.6)
project(cryptography)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8;/D_CRT_SECURE_NO_WARNINGS>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8;/D_CRT_SECURE_NO_WARNINGS>")

add_subdirectory(src)
add_subdirectory(demo)
