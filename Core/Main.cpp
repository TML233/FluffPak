#include "Core/Definition.h"
#include "Core/System/Console.h"
#include "Core/Collection/Dictionary.h"
#include "Core/System/String.h"
using namespace Core;

int main() {
	Console::Initialize();
	Dictionary<int, int> dic{};
	dic.Add(1, 2);
	Console::PrintLine(Object::ToString(dic[1]));
	
	return 0;
}