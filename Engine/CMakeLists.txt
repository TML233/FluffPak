cmake_minimum_required(VERSION 3.8)
project("Engine" LANGUAGES CXX)

# Build Engine
add_library(Engine STATIC)
target_include_directories(Engine PUBLIC "./")
# Link thirdparty libraries
target_link_libraries(Engine PUBLIC fmt)

# Add common source/header files.
set(SourceFileCore
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Object.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/String.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Memory.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Debug.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Math.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Random.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Vector2.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Vector3.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Algorithm/StringSearcherSunday.cpp"
	
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Collection/HashHelper.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node.cpp"
)
set(HeaderFileCore
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Definition.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Object.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/String.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/UniquePtr.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/SharedPtr.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Memory.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Debug.h"
	
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Math.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Random.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Vector2.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Vector3.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Algorithm/StringSearcherSunday.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Collection/Iterator.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Collection/HashHelper.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Collection/List.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Application/Node.h"
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