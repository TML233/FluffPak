cmake_minimum_required(VERSION 3.8)
project("Engine" LANGUAGES CXX)

# Add thirdparty library projects
add_subdirectory("Thirdparty/fmt/")

# Build Engine
add_library(Engine STATIC)
target_include_directories(Engine PUBLIC "./")
# Link thirdparty libraries
target_link_libraries(Engine PRIVATE fmt)

# Add common source/header files.
set(SourceFileCore
	"${CMAKE_CURRENT_LIST_DIR}/Core/System/Object.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Core/System/Exception.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Core/System/String.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Core/System/Console.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Core/Math/Math.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Core/Math/Vector2.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Core/Math/Vector3.cpp"
	
	"${CMAKE_CURRENT_LIST_DIR}/Core/Collection/HashHelper.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Core/Engine/Engine.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Core/Engine/AppLoop.cpp"
)
set(HeaderFileCore
	"${CMAKE_CURRENT_LIST_DIR}/Core/System/Definition.h"
	"${CMAKE_CURRENT_LIST_DIR}/Core/System/Object.h"
	"${CMAKE_CURRENT_LIST_DIR}/Core/System/Exception.h"
	"${CMAKE_CURRENT_LIST_DIR}/Core/System/String.h"
	"${CMAKE_CURRENT_LIST_DIR}/Core/System/Console.h"
	"${CMAKE_CURRENT_LIST_DIR}/Core/System/IEnumerable.h"
	"${CMAKE_CURRENT_LIST_DIR}/Core/System/UniquePtr.h"
	"${CMAKE_CURRENT_LIST_DIR}/Core/System/Allocator.h"

	"${CMAKE_CURRENT_LIST_DIR}/Core/Math/Math.h"
	"${CMAKE_CURRENT_LIST_DIR}/Core/Math/Vector2.h"
	"${CMAKE_CURRENT_LIST_DIR}/Core/Math/Vector3.h"

	"${CMAKE_CURRENT_LIST_DIR}/Core/Collection/Iterator.h"
	"${CMAKE_CURRENT_LIST_DIR}/Core/Collection/List.h"
	"${CMAKE_CURRENT_LIST_DIR}/Core/Collection/HashHelper.h"
	"${CMAKE_CURRENT_LIST_DIR}/Core/Collection/Dictionary.h"

	"${CMAKE_CURRENT_LIST_DIR}/Core/Engine/Engine.h"
	"${CMAKE_CURRENT_LIST_DIR}/Core/Engine/AppLoop.h"
	"${CMAKE_CURRENT_LIST_DIR}/Core/Engine/FileSystem.h"
)

# Add platform-depended source files.
if(WIN32)
	# Use Windows Unicode
	target_compile_options(Engine PUBLIC -DUNICODE -D_UNICODE)
elseif(__LINUX__)

endif()

# Build with sources and headers.
target_sources(Engine
	PUBLIC ${HeaderFileCore} ${HeaderFilePlatform}
	PRIVATE ${SourceFileCore} ${SourceFilePlatform}
)