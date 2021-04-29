#include "Engine/System/Regex.h"

namespace Engine {
	Regex::MatchRange::MatchRange(int32 from, int32 to) :from(from), to(to) {}
	bool Regex::MatchRange::IsValid() const {
		return (from >= 0 && to >= from);
	}

	Regex::Result Regex::CreateRegex(const String& pattern, std::regex& result) {
		std::string_view svp = pattern.GetStringView();
		std::regex rp;
		try {
			rp = std::regex(svp.begin(), svp.end());
		} catch (const std::regex_error& err) {
			using Error = std::regex_constants::error_type;
			switch (err.code()) {
				case Error::error_collate:
				case Error::error_ctype:
				case Error::error_escape:
				case Error::error_backref:
				case Error::error_brack:
				case Error::error_paren:
				case Error::error_brace:
				case Error::error_badbrace:
				case Error::error_range:
				case Error::error_badrepeat:
					ERR_MSG("Invalid regex expression.");
					return Result::ExpressionError;

				case Error::error_space:
				case Error::error_stack:
					ERR_MSG("Out of memory.");
					return Result::OutOfMemory;

				case Error::error_complexity:
					ERR_MSG("Regex expression too complex.");
					return Result::ExpressionTooComplex;

				default:
					ERR_MSG(String::Format("Unknown error code {0} when constructing regex expression.", (int32)err.code()).GetRawArray());
					return Result::UnknownError;
			}
		}

		result = rp;
		return Result::OK;
	}

	Regex::Result Regex::Match(const String& content, const String& pattern, List<Regex::MatchRange>& results) {
		// Construct regex expression
		std::regex expression;
		Regex::Result expressionResult = CreateRegex(pattern, expression);
		ERR_ASSERT(expressionResult == Result::OK, "Failed to construct regex expression.", return expressionResult);

		// Get ready for matching
		std::string_view contentSv = content.GetStringView();
		using TIter = decltype(contentSv.cbegin());

		std::regex_iterator<TIter> iter(contentSv.cbegin(), contentSv.cend(),expression);
		std::regex_iterator<TIter> end;

		// Clear result list.
		results.Clear();

		// Match and iterate all matches
		while (iter != end) {
			const auto& matches = *iter;
			for (sizeint i = 0; i < matches.size(); ++i) {
				const auto& match = matches[i];
				results.Add(MatchRange(match.first - contentSv.begin(), match.second - contentSv.begin()));
			}
			++iter;
		}

		return Result::OK;
	}
}
