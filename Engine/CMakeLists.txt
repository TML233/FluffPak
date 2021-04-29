cmake_minimum_required(VERSION 3.8)
project("Engine" LANGUAGES CXX)

# Build Engine
add_library(Engine STATIC)

# Define common header/source files.
set(HeaderFile
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Definition.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Object.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/ReferencePtr.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/ObjectUtil.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/InstanceId.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/String.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/UniquePtr.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/SharedPtr.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Memory.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Debug.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Atomic.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Variant.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/File.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Regex.h"
	
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Math.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Random.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Vector2.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Rect2.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Transform2.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Collection/Iterator.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Collection/HashHelper.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Collection/List.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Collection/Dictionary.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Definition.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Platform.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/NativeWindow.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Engine.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Time.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/AppLoop.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/NativeWindow.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/NodeTree.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/NodePath.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/Node.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/Node2D.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/TestNode.h"
)
set(SourceFile
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Object.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/ObjectUtil.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/InstanceId.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/String.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Memory.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Debug.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Variant.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/File.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Regex.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Math.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Random.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Vector2.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Rect2.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Transform2.cpp"
	
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Collection/HashHelper.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Engine.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Time.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/AppLoop.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/NativeWindow.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/NodeTree.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/NodePath.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/Node.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/Node2D.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/TestNode.cpp"
)
set(IncludeDir
	"${CMAKE_CURRENT_LIST_DIR}"
)
set(LinkLibrary
	fmt
)

# Add platform-depended header/source files.
if(WIN32)
	list(APPEND HeaderFile
		"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Windows/Resource/Resource.h"
		"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Windows/Resource/TargetVer.h"

		"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Windows/BetterWindows.h"
		"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Windows/UnicodeHelper.h"

		"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Windows/NativeWindow.h"
	)
	list(APPEND SourceFile
		"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Windows/Resource/Resource.rc"

		"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Windows/UnicodeHelper.cpp"

		"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Windows/NativeWindow.cpp"
	)
	list(APPEND IncludeDir
	)
	list(APPEND LinkLibrary
	)
elseif(UNIX AND "${CMAKE_SYSTEM}" MATCHES "Linux")
	list(APPEND HeaderFile
		"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Linux/NativeWindow.h"
	)
	list(APPEND SourceFile
		"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Linux/NativeWindow.cpp"
	)

	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK REQUIRED gtk+-3.0)

	list(APPEND IncludeDir
		"${GTK_INCLUDE_DIRS}"
	)
	list(APPEND LinkLibrary
		"${GTK_LIBRARIES}"
	)
endif()

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	target_compile_options(Engine PUBLIC -Wno-switch)
endif()

if(MSVC)
	target_compile_options(Engine PUBLIC /utf-8)
endif()

# Force C++17
target_compile_features(Engine PUBLIC cxx_std_17)

# Include directories
target_include_directories(Engine PUBLIC ${IncludeDir})

# Link thirdparty libraries
target_link_libraries(Engine PUBLIC ${LinkLibrary})

# Build with sources and headers.
target_sources(Engine
	PUBLIC ${HeaderFile}
	PRIVATE ${SourceFile}
)