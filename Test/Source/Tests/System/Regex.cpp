#include "doctest.h"
#include "Engine/System/Regex.h"

using namespace Engine;

TEST_SUITE("Regex") {
	TEST_CASE("Regex") {
		String content = STRING_LITERAL(u8"CAONIMA SBSBSB 12 123 asd");
		String pattern = STRING_LITERAL(u8"[0-9]+");

		List<Regex::MatchRange> matches{};
		Regex::Match(content, pattern, matches);

		CHECK(matches.GetCount() == 2);

		auto match = matches.Get(0);
		int32 index = content.IndexOf(STRING_LITERAL(u8"12"));
		CHECK(match.from == index);
		CHECK(match.to == index+2);

		match = matches.Get(1);
		index = content.IndexOf(STRING_LITERAL(u8"123"));
		CHECK(match.from == index);
		CHECK(match.to == index+3);
	}
}
