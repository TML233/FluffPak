cmake_minimum_required(VERSION 3.8)
project("glad" LANGUAGES C)

add_library(glad STATIC)
target_include_directories(glad PUBLIC "include/")
target_sources(glad
	PUBLIC
		"${CMAKE_CURRENT_LIST_DIR}/include/glad/glad.h"
		"${CMAKE_CURRENT_LIST_DIR}/include/KHR/khrplatform.h"
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/src/glad.c"
)