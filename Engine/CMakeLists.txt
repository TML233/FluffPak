cmake_minimum_required(VERSION 3.8)
project("Engine" LANGUAGES CXX)

# Add thirdparty library projects
add_subdirectory("Thirdparty/fmt/")

add_library(Engine STATIC)
target_include_directories(Engine PUBLIC "./")
target_link_libraries(Engine PRIVATE fmt)

# Add common source/header files.
set(SourceFileCore
	"Core/System/Object.cpp"
	"Core/System/Exception.cpp"
	"Core/System/String.cpp"
	"Core/System/Console.cpp"
	"Core/Math/Math.cpp"
	"Core/Math/Vector2.cpp"
	"Core/Math/Vector3.cpp"
	"Core/Engine/AppLoop.cpp"
	"Core/Engine/Engine.cpp"
	"Core/Collection/HashHelper.cpp"
)
set(HeaderFileCore "")

# Add platform-depended source files.
if(WIN32)
	# Use Windows Unicode
	target_compile_options(Engine PUBLIC -DUNICODE -D_UNICODE)
elseif(__LINUX__)

endif()

target_sources(Engine
	PUBLIC ${HeaderFileCore} ${HeaderFilePlatform}
	PRIVATE ${SourceFileCore} ${SourceFilePlatform}
)