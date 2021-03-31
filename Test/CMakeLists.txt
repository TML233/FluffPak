cmake_minimum_required(VERSION 3.8)
project("Test" LANGUAGES CXX)

add_executable(Test)
target_link_libraries(Test PRIVATE Engine)
target_link_libraries(Test PRIVATE doctest)
target_sources(Test PRIVATE
	"${CMAKE_CURRENT_LIST_DIR}/Source/Main.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Source/Memory.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Source/String.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Source/List.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Source/Dictionary.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Source/Variant.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Source/Reflection.cpp"
)
if(MSVC)
	target_compile_options(Test PRIVATE /utf-8)
endif()