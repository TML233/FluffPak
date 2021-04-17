cmake_minimum_required(VERSION 3.8)
project("Test" LANGUAGES CXX)

add_executable(Test)
target_link_libraries(Test PRIVATE Engine)
target_link_libraries(Test PRIVATE doctest)
target_sources(Test PRIVATE
	"${CMAKE_CURRENT_LIST_DIR}/Source/Main.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Source/Tests/System/Memory.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Source/Tests/System/String.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Source/Tests/System/Variant.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Source/Tests/System/Reflection.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Source/Tests/Collection/List.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Source/Tests/Collection/Dictionary.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Source/Tests/Math/Transform2.cpp"
)
if(MSVC)
	target_compile_options(Test PRIVATE /utf-8)
endif()