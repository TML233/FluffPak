#include "Core/Definition.h"
#include "Core/System/Console.h"
#include "Core/Template/List.h"
#include "Core/Math/Vector2.h"
#include "Core/System/String.h"
using namespace Core;

int main() {
	String str = "a";
	String str2 = str+"baa";
	Console::PrintLine(str2);
	Console::PrintLine(str);
	List<int> a;
	Console::PrintLine(String::Format("Test{0}",1,2));
	return 0;
}