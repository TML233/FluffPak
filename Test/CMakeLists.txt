cmake_minimum_required(VERSION 3.8)
project("Test")

include_directories("Include/")

# Add test source files.
set(SourceFiles
"Main.cpp"
)

add_executable(Test ${SourceFiles})