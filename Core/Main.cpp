#include "Core/Definition.h"
#include "Core/System/Console.h"
#include "Core/Template/List.h"
#include "Core/Math/Vector2.h"
#include "Core/System/String.h"
#include "Core/Template/UniquePtr.h"
#include "Core/Engine/Engine.h"
using namespace Core;

int main() {
	Console::Initialize();

	Object obj{};
	Console::PrintLine(Object::ToString(obj.GetHashCode()));
	return 0;
}