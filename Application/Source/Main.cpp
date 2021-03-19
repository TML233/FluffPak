#include "Engine/System/Debug.h"
#include "Engine/System/Object.h"
#include "Engine/Collection/List.h"
using namespace Engine;

int main() {
	List<int> list{ 1,2,3 };
	list.Insert(100, 3);
	list.Get(5);
}
