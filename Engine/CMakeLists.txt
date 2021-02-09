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
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Exception.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/String.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Console.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/TimeSpan.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Math.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Random.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Vector2.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Vector3.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Algorithm/StringSearcherSunday.cpp"
	
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Collection/HashHelper.cpp"
)
set(HeaderFileCore
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Definition.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Object.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Exception.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/String.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Console.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/IEnumerable.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/UniquePtr.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/Allocator.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/System/TimeSpan.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Math.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Random.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Vector2.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Math/Vector3.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Algorithm/StringSearcherSunday.h"

	"${CMAKE_CURRENT_LIST_DIR}/Engine/Collection/Iterator.h"
	"${CMAKE_CURRENT_LIST_DIR}/Engine/Collection/HashHelper.h"	
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