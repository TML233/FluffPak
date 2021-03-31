#pragma once

#include "fmt/core.h"
#include "fmt/format.h"
#include "Engine/Algorithm/StringSearcherSunday.h"
#include "Engine/Collection/Iterator.h"
#include "Engine/System/Atomic.h"
#include "Engine/System/ReferencePtr.h"
#include <string_view>

// Defines a String literal.
#define STRING_LITERAL(text)																							\
([](){																													\
	static const char content[]=text;																					\
	static const ::Engine::StringData data(content, sizeof(content), true);												\
	return ::Engine::String(::Engine::ReferencePtr<::Engine::StringData>(const_cast<::Engine::StringData*>(&data)));	\
})()

namespace Engine {
	struct StringData final {
		// Will allocate memory and copy from data.
		// If staticData == true, will use pre-defined string data instead of copying.
		StringData(const char* data, int32 length, bool staticData = false);

		~StringData();

		// Original data of the string.
		const char* data = nullptr;
		// NULL included.
		int length = 0;

		uint32 Reference();
		uint32 Dereference();
		uint32 GetReferenceCount() const;

		static StringData* GetEmpty();
	private:
		bool staticData;
		ReferenceCount referenceCount;
	};

	// A string holding a NULL-termined char array.
	class String final {
	public:
		String(const char* string = "");
		String(const std::string& string);
		String& operator=(const char* string);

		// For STRING_LITERAL
		// DO NOT use.
		String(ReferencePtr<StringData> dataPtr);

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
		// If count == -1, search the whole string.
		// Return -1 if not found.
		int32 IndexOf(const String& pattern,int32 startFrom=0,int32 count=-1) const;

		// Check if the current string contains another string.
		bool Contains(const String& pattern) const;

		bool StartsWith(const String& pattern) const;
		bool EndsWith(const String& pattern) const;
		String Substring(int32 startIndex, int32 count) const;

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
		int32 GetHashCode() const;

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
		ReferencePtr<StringData> data;

		int32 refStart = 0;
		int32 refCount = 0;

		// Global sunday string searcher.
		static StringSearcherSunday searcher;
	};

	String operator+(const String& left, const String& right);
}

namespace fmt {
	template<>
	struct formatter<::Engine::String> : formatter<string_view> {
		template <typename FormatContext>
		auto format(const ::Engine::String& c, FormatContext& ctx) {
			return formatter<string_view>::format(string_view(c.GetStartPtr(), c.GetCount()), ctx);
		}
	};
}