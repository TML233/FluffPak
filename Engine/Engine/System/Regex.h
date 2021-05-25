#pragma once
#include "Engine/System/String.h"
#include "Engine/System/Collection/List.h"
#include <regex>

namespace Engine {
	class Regex final {
	public:
		struct MatchRange {
			MatchRange();
			MatchRange(int32 from, int32 to);
			int32 from;
			int32 to;
			bool IsValid() const;
		};


		static ResultCode Match(const String& content, const String& pattern, List<MatchRange>& results);

	private:
		static ResultCode CreateRegex(const String& pattern, std::regex& result);
	};
}
