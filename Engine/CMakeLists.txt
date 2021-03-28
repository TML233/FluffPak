cmake_minimum_required(VERSION 3.8)
project("Engine" LANGUAGES CXX)

# Build Engine
add_library(Engine STATIC)
target_include_directories(Engine PUBLIC "./")
# Link thirdparty libraries
target_link_libraries(Engine PUBLIC fmt)

# Add common header/source files.
set(HeaderFileCore
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Definition.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Object.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/ObjectUtil.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/InstanceId.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/String.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/UniquePtr.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/SharedPtr.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/ReferencePtr.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Memory.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Debug.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Atomic.h"
	
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Math.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Random.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Vector2.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Rect2.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Algorithm/StringSearcherSunday.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Collection/Iterator.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Collection/HashHelper.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Collection/List.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Collection/Dictionary.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Engine.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Time.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/AppLoop.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/NodeTree.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/NodePath.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/Node.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/Node2D.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/Rect2D.h"
)
set(SourceFileCore
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Object.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/ObjectUtil.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/InstanceId.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/String.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Memory.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Debug.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Math.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Random.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Vector2.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Rect2.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Algorithm/StringSearcherSunday.cpp"
	
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Collection/HashHelper.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Engine.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Time.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/AppLoop.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/NodeTree.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/NodePath.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/Node.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/Node2D.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node/Rect2D.cpp"
)

# Add platform-depended source files.
if(WIN32)
	# Use Windows Unicode
	target_compile_options(Engine PUBLIC -DUNICODE -D_UNICODE)
elseif(__LINUX__)

endif()

# Force C++17
target_compile_features(Engine PUBLIC cxx_std_17)

# Build with sources and headers.
target_sources(Engine
	PUBLIC ${HeaderFileCore} ${HeaderFilePlatform}
	PRIVATE ${SourceFileCore} ${SourceFilePlatform}
)