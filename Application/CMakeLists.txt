cmake_minimum_required(VERSION 3.8)
project("Application" LANGUAGES CXX)

add_executable(Application)
target_link_libraries(Application PRIVATE Engine)
target_link_libraries(Application PRIVATE glm)

set(HeaderFile
	"${CMAKE_CURRENT_LIST_DIR}/Application/GLShader.h"
	"${CMAKE_CURRENT_LIST_DIR}/Application/TestNode.h"
)
set(SourceFile
	"${CMAKE_CURRENT_LIST_DIR}/Application/Main.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Application/GLShader.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Application/TestNode.cpp"
)

if(WIN32)

endif()

if(MSVC)
	target_compile_options(Application PRIVATE /utf-8)
	list(APPEND HeaderFile
		"${CMAKE_CURRENT_LIST_DIR}/../Engine/Engine/Platform/Windows/Resource/Resource.h"
		"${CMAKE_CURRENT_LIST_DIR}/../Engine/Engine/Platform/Windows/Resource/TargetVer.h"
	)
	list(APPEND SourceFile
		"${CMAKE_CURRENT_LIST_DIR}/../Engine/Engine/Platform/Windows/Resource/Resource.rc"
	)
endif()

target_include_directories(Application PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

target_sources(Application PRIVATE
	"${HeaderFile}" "${SourceFile}"
)