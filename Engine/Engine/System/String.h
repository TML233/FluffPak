#pragma once

#include "fmt/core.h"
#include "fmt/format.h"
#include "Engine/Algorithm/StringSearcherSunday.h"
#include "Engine/Collection/Iterator.h"
#include "Engine/System/Atomic.h"
#include "Engine/System/UniquePtr.h"
#include "Engine/System/ReferencePtr.h"
#include "Engine/Collection/List.h"
#include <string_view>

// Defines a String literal.
#define STRING_LITERAL(text)																							\
([](){																													\
	static const char content[]=text;																					\
	static const ::Engine::String::ContentData data(content, sizeof(content));												\
	return ::Engine::String(::Engine::ReferencePtr<::Engine::String::ContentData>(const_cast<::Engine::String::ContentData*>(&data)));	\
})()

namespace Engine {
	// A string holding a NULL-termined char array.
	class String final {
	public:
		// Container of actual data of Strings. Shared between Strings.
		// Does not copy any data.
		struct ContentData final {
			// Accept data as a static block. Will not free the data.
			// Only for internal use. Used by STRING_LITERAL.
			// Must NOT be allocated on heap, or the memory leak would occur.
			ContentData(const char* data, int32 length);

			// Accept data as a memory block on heap. Will free the data.
			ContentData(UniquePtr<char[]>&& data, int32 length);

			~ContentData();

			// Original data of the string.
			const char* data = nullptr;
			// NULL included.
			int length = 0;

			uint32 Reference() const;
			uint32 Dereference() const;
			uint32 GetReferenceCount() const;

			static ReferencePtr<ContentData> GetEmpty();
		private:
			bool staticData;
			mutable ReferenceCount referenceCount;
		};

		static String GetEmpty();

#pragma region ctor/assignment
		// Creates a string from a raw string.
		// Will copy the original string.
		String(const char* string = "", int32 count = -1);
		
		String& operator=(const char* string);

		// Creates a string from a std::string.
		// Will copy the original string.
		String(const std::string& string);

		// Creates a string using a ContentData.
		// Will not copy.
		String(ReferencePtr<ContentData> dataPtr, int32 start = 0, int32 count = -1);
#pragma endregion

#pragma region Tool functions
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
		String Replace(const String& from, const String& to) const;

		String ToString() const;
		int32 GetHashCode() const;

		int32 GetStartIndex() const;
		const char* GetStartPtr() const;

		bool operator==(const String& obj) const;
		bool operator!=(const String& obj) const;
#pragma endregion

#pragma region Format
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
#pragma endregion

		String operator+(const String& obj);

	private:
		bool IsEqual(const String& obj) const;

		// Prepares a string, the string data will be copied.
		// Count does not accept -1.
		void PrepareData(const char* string, sizeint count);

		// Current data reference.
		ReferencePtr<ContentData> data;

		int32 refStart = 0;
		int32 refCount = 0;

		// Global sunday string searcher.
		static StringSearcherSunday searcher;

		static List<sizeint> replacerIndexes;
	};
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