cmake_minimum_required (VERSION 3.8)
project ("Engine")

include_directories("./")


# Add common source files.
set(SourceFilesCore
"Core/Main.cpp"
"Core/System/Object.cpp"
"Core/System/Exception.cpp"
"Core/System/String.cpp"
"Core/System/Console.cpp"
"Core/Math/Math.cpp"
"Core/Math/Vector2.cpp"
"Core/Math/Vector3.cpp"
"Core/Engine/AppLoop.cpp"
"Core/Engine/Engine.cpp")


# Add thirdparty include paths.
include_directories("Thirdparty/fmt/include/")

# Add thirdparty source files.
set(SourceFilesThirdparty
	"Thirdparty/fmt/src/format.cc"
)


# Add platform-depended source files.
set(SourceFilesPlatform "")

if(WIN32)
	set(SourceFilesPlatform
	#"Platform/Windows/OS/Window.cpp"
	#"Platform/Windows/UnicodeHelper.cpp"
	)
	# Use Windows Unicode
	add_definitions(-DUNICODE -D_UNICODE)
elseif(__LINUX__)
	set(SourceFilesPlatform "")
endif()


add_executable(Engine ${SourceFilesThirdparty} ${SourceFilesCore} ${SourceFilesPlatform})