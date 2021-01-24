cmake_minimum_required(VERSION 3.8)
project("fmt" LANGUAGES CXX)

add_library(fmt STATIC)
target_include_directories(fmt PUBLIC "include/")
target_sources(fmt
PUBLIC
"${CMAKE_CURRENT_LIST_DIR}/include/fmt/core.h"
"${CMAKE_CURRENT_LIST_DIR}/include/fmt/format.h"
"${CMAKE_CURRENT_LIST_DIR}/include/fmt/format-inl.h"
PRIVATE
"${CMAKE_CURRENT_LIST_DIR}/src/format.cc"
)