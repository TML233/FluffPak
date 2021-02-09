#include "doctest.h"
#include "Engine/System/Exception.h"

TEST_CASE("Exception") {
	bool cought = false;
	try {
		throw NotImplementedException("Test", "Fuck");
	} catch (NotImplementedException e) {
		//MESSAGE(e.ToString().GetRawArray());
		cought = true;
	}
	CHECK(cought);
}