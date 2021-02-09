cmake_minimum_required(VERSION 3.8)
project("doctest" LANGUAGES CXX)

add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE "Include/")
target_sources(doctest
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/Include/doctest.h"
)