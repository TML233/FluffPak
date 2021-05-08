#include "Engine/System/Regex.h"

namespace Engine {
	Regex::MatchRange::MatchRange() :from(-1), to(-1) {}
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

#if defined(_MSC_VER)
#	define REGEX_ERROR(x) std::regex_constants::error_type::x
#else
#	define REGEX_ERROR(x) std::regex_constants::x
#endif

			switch (err.code()) {
				case REGEX_ERROR(error_collate):
				case REGEX_ERROR(error_ctype):
				case REGEX_ERROR(error_escape):
				case REGEX_ERROR(error_backref):
				case REGEX_ERROR(error_brack):
				case REGEX_ERROR(error_paren):
				case REGEX_ERROR(error_brace):
				case REGEX_ERROR(error_badbrace):
				case REGEX_ERROR(error_range):
				case REGEX_ERROR(error_badrepeat):
					ERR_MSG(u8"Invalid regex expression.");
					return Result::ExpressionError;

				case REGEX_ERROR(error_space):
				case REGEX_ERROR(error_stack):
					ERR_MSG(u8"Out of memory.");
					return Result::OutOfMemory;

				case REGEX_ERROR(error_complexity):
					ERR_MSG(u8"Regex expression too complex.");
					return Result::ExpressionTooComplex;

				default:
					ERR_MSG(String::Format(u8"Unknown error code {0} when constructing regex expression.", (int32)err.code()).GetRawArray());
					return Result::UnknownError;
			}
#undef REGEX_ERROR
		}

		result = rp;
		return Result::OK;
	}

	Regex::Result Regex::Match(const String& content, const String& pattern, List<Regex::MatchRange>& results) {
		// Construct regex expression
		std::regex expression;
		Regex::Result expressionResult = CreateRegex(pattern, expression);
		ERR_ASSERT(expressionResult == Result::OK, u8"Failed to construct regex expression.", return expressionResult);

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
