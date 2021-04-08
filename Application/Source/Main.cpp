#include "Engine/System/Debug.h"
#include "Engine/Application/Engine.h"
#include "Engine/Application/Node/NodeTree.h"
#include "Engine/System/Variant.h"

using namespace Engine;
using var = Variant;

int main() {
	var a = 4;
	var b = 3.99;
	var c = STRING_LITERAL(" YYDS");
	var d = a + b + c + true + var();

	printf("%s", d.ToString().GetRawArray());

	return 0;
}