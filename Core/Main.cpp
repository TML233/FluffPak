#include "Core/Definition.h"
#include "Core/OS/Console.h"
#include "Core/Template/List.h"
#include "Core/Math/Vector2.h"
#include "Core/System/String.h"
#include "Core/Template/UniquePtr.h"
using namespace Core;

int main() {
	String str1 = "a";
	String str2 = str1;
	Console::PrintLine(Object::ToString(str1.GetRawArray() == str2.GetRawArray()));

	String empty1 = "";
	String empty2 = "";
	Console::PrintLine(Object::ToString(empty1.GetRawArray() == empty2.GetRawArray()));

	UniquePtr<int> uptr(new int());
	UniquePtr<int[]> uptr2(new int[3]());
	return 0;
}