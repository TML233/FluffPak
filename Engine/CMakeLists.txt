cmake_minimum_required(VERSION 3.8)
project("Engine" LANGUAGES CXX)

# Build Engine
add_library(Engine STATIC)

# Define common header/source files.
set(HeaderFile
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Definition.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Object/Object.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Object/ObjectUtil.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Object/InstanceId.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Object/Reflection.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Object/Variant.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Memory/Memory.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Memory/UniquePtr.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Memory/SharedPtr.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Memory/IntrusivePtr.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Memory/CopyOnWrite.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Thread/ThreadUtil.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Thread/Atomic.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Thread/JobSystem.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Stream.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/String.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Debug.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Regex.h"
	
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/File/FileStream.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/File/FileProtocol.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/File/FileSystem.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/File/Protocol/Native.h"
	
	
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Math/Math.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Math/Random.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Math/Vector2.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Math/Rect2.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Math/Transform2.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Collection/Iterator.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Collection/HashHelper.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Collection/List.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Collection/Dictionary.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Collection/Deque.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Definition.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Platform.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Window.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Engine.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Time.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/AppLoop.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Window.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/NodeTree.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/NodePath.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/Node.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/Node2D.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/TestNode.h"
)
set(SourceFile
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Object/Object.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Object/ObjectUtil.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Object/InstanceId.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Object/Reflection.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Object/Variant.cpp"
	
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Memory/Memory.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Thread/ThreadUtil.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Thread/JobSystem.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Stream.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/String.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Debug.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Regex.cpp"
	
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/File/FileStream.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/File/FileProtocol.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/File/FileSystem.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/File/Protocol/Native.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Math/Math.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Math/Random.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Math/Vector2.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Math/Rect2.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Math/Transform2.cpp"
	
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Collection/HashHelper.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Engine.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Time.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/AppLoop.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Window.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/NodeTree.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/NodePath.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/Node.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/Node2D.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/TestNode.cpp"
)
set(InterfaceFile
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
		"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Windows/BetterWindows.h"
		"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Windows/UnicodeHelper.h"

		"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Windows/Window.h"
	)
	list(APPEND SourceFile
		"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Windows/UnicodeHelper.cpp"

		"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Windows/Window.cpp"
	)
	list(APPEND InterfaceFile
		#"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Windows/Resource/Resource.h"
		#"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Windows/Resource/TargetVer.h"
		#"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Windows/Resource/Resource.rc"
	)
	list(APPEND IncludeDir
	)
	list(APPEND LinkLibrary
		Shcore
	)
elseif(UNIX AND "${CMAKE_SYSTEM}" MATCHES "Linux")
	list(APPEND HeaderFile
		"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Linux/Window.h"
	)
	list(APPEND SourceFile
		"${CMAKE_CURRENT_LIST_DIR}/Engine/Platform/Linux/Window.cpp"
	)
	list(APPEND InterfaceFile
	)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK REQUIRED gtk+-3.0)

	list(APPEND IncludeDir
		"${GTK_INCLUDE_DIRS}"
	)
	list(APPEND LinkLibrary
		"${GTK_LIBRARIES}" pthread
	)
endif()

if(MSVC)
	target_compile_options(Engine PUBLIC /utf-8)
	target_compile_options(Engine PUBLIC /std:c++latest)
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	target_compile_options(Engine PUBLIC -std=c++2a)
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
	target_compile_options(Engine PUBLIC -Wno-switch)
endif()

# Force C++20
target_compile_features(Engine PUBLIC cxx_std_20)

# Include directories
target_include_directories(Engine PUBLIC ${IncludeDir})

# Link thirdparty libraries
target_link_libraries(Engine PUBLIC ${LinkLibrary})

# Build with sources and headers.
target_sources(Engine
	PUBLIC ${HeaderFile}
	PRIVATE ${SourceFile}
	INTERFACE ${InterfaceFile}
)