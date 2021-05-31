cmake_minimum_required(VERSION 3.8)
project("glm" LANGUAGES C)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE "./")
