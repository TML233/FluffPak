cmake_minimum_required(VERSION 3.8)
project("Test" LANGUAGES CXX)

add_executable(Test)
target_include_directories(Test PRIVATE "Include/")
target_sources(Test PRIVATE
"${CMAKE_CURRENT_LIST_DIR}/Main.cpp"
)