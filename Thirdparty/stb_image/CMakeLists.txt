cmake_minimum_required(VERSION 3.8)
project("stb_image" LANGUAGES CXX)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE "Include/")
target_sources(stb_image
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/Include/stb_image.h"
)