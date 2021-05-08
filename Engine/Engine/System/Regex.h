#pragma once
#include "Engine/System/String.h"
#include "Engine/Collection/List.h"
#include <regex>

namespace Engine {
	class Regex final {
	public:
		enum class Result {
			OK,
			UnknownError,
			ExpressionError,
			ExpressionTooComplex,
			OutOfMemory,
		};

		struct MatchRange {
			MatchRange();
			MatchRange(int32 from, int32 to);
			int32 from;
			int32 to;
			bool IsValid() const;
		};

		//static ResultPair<Result,bool> IsMatch(const String& content, const String& pattern);
		static Result Match(const String& content, const String& pattern, List<MatchRange>& results);

	private:
		static Result CreateRegex(const String& pattern, std::regex& result);
	};
}
