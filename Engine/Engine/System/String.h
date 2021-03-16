#pragma once

#include <memory>
#include "fmt/core.h"
#include "fmt/format.h"
#include "Engine/Algorithm/StringSearcherSunday.h"
#include <string_view>

// Defines a String literal. Encoded in UTF-8.
#define STRING_LITERAL(text)									\
([](){															\
	static const char textContent[]=u8##text##;					\
	static const StringData literal={							\
		textContent,											\
		(int32)sizeof(u8##text##)								\
	};															\
	/* TODO: Use dummy sharedptr */								\
})();

namespace Engine {
	template<typename T>
	class ReadonlyIterator;
	template<typename T>
	class List;

	struct StringData final {
		explicit StringData(const char* data,int32 count);
		~StringData();

		// Original data of the string.
		char* data = nullptr;
		// NULL included.
		int length = 0;

		static std::shared_ptr<StringData> empty;
	};

	// A string holding a NULL-termined char array.
	class String final {
	public:
		String(const char* string = "");
		String(const std::string& string);
		String& operator=(const char* string);

		// Get char count.
		// NULL not included.
		int32 GetCount() const;

		// Get internal C-style char array.
		// Do not store the pointer.
		const char* GetRawArray() const;

		bool IsIndividual() const;
		String ToIndividual() const;

		ReadonlyIterator<char> operator[](int32 index) const;

		// Find the position of the substring appearance in the current string.
		// Return -1 if not found.
		int32 IndexOf(const String& pattern) const;

		bool StartsWith(const String& pattern) const;
		bool EndsWith(const String& pattern) const;
		String Substring(int32 startIndex, int32 count) const;
		String Trim() const;
		String TrimStart() const;
		String TrimEnd() const;

		// Format strings like "This is a {0}, except {1}";
		template<typename ... Ts>
		static String Format(const String& format, const Ts& ... args) {
			return fmt::format(format.ToIndividual().GetRawArray(), args...);
		}
		// Format strings like "This is a {0}, except {1}";
		template<typename ... Ts>
		static String Format(const char* format, const Ts& ... args) {
			return fmt::format(format, args...);
		}

		String ToString() const;

		int32 GetStartIndex() const;
		const char* GetStartPtr() const;

		bool operator==(const String& obj) const;
		bool operator!=(const String& obj) const;

	private:
		bool IsEqual(const String& obj) const;

		// Internal ctor
		String(const char* string, int32 count);
		// Prepares a string with a brand new data object.
		void PrepareData(const char* string, int32 count);
		// Current data reference.
		std::shared_ptr<StringData> data;

		int32 refStart = 0;
		int32 refCount = 0;

		// Global sunday string searcher.
		static StringSearcherSunday searcher;
	};

	String operator+(const String& left, const String& right);
}

using namespace Engine;
namespace fmt {
	template<>
	struct formatter<String> : formatter<string_view> {
		template <typename FormatContext>
		auto format(const String& c, FormatContext& ctx) {
			return formatter<string_view>::format(string_view(c.GetStartPtr(), c.GetCount()), ctx);
		}
	};
}