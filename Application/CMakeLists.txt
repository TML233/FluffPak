cmake_minimum_required(VERSION 3.8)
project("Application" LANGUAGES CXX)

add_executable(Application)
target_link_libraries(Application PRIVATE Engine)
target_sources(Application PRIVATE
	"${CMAKE_CURRENT_LIST_DIR}/Source/Main.cpp"
)
if(MSVC)
	target_compile_options(Application PRIVATE /utf-8)
endif()