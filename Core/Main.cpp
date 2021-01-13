#include "Core/Definition.h"
#include "Core/System/Console.h"
#include "Core/Template/List.h"
#include "Core/Math/Vector2.h"
#include "Core/System/String.h"
#include "Core/Template/UniquePtr.h"
using namespace Core;

int main() {
	Console::Initialize();
	Console::PrintLine(u8"Test, 测试, 曰你妈Windows");
	return 0;
}