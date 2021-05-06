#pragma once

#include "fmt/core.h"
#include "fmt/format.h"
#include "Engine/Collection/Iterator.h"
#include "Engine/System/Atomic.h"
#include "Engine/System/Memory/UniquePtr.h"
#include "Engine/System/Object/ReferencePtr.h"
#include "Engine/Collection/List.h"
#include <string_view>

/// @brief Make a String literal.
/// This prevents allocating heap memory for constant strings.
#define STRING_LITERAL(text)																											\
([](){																																	\
	static const u8char content[]=text;																									\
	static const ::Engine::String::ContentData data(content, sizeof(text));																\
	return ::Engine::String(::Engine::ReferencePtr<::Engine::String::ContentData>(const_cast<::Engine::String::ContentData*>(&data)));	\
})()

/// @brief Short alias for STRING_LITERAL
#define STRL STRING_LITERAL

namespace Engine {
	/// @brief A string holding a NULL-termined char array.
	/// The actual content is reference counted, so it's cheap to copy around.
	class String final {
	public:
		/// @brief Container of actual content data of Strings. Shared between Strings. 
		struct ContentData final {
			/// @brief Accept data as a static block. Will not free the data.
			///	Only for internal use. Used by STRING_LITERAL.
			/// @param data A static string data block. Can't be allocated on heap.
			/// @param length The length of the given string data block. NULL included.
			ContentData(const u8char* data, int32 length);

			/// @brief Accept data as a memory block on heap. Will free the data.
			/// @param data A UniquePtr holding string data block on heap. Use Memory::Move to "move" it in.
			/// @param length The length of the given string data block. NULL included.
			ContentData(UniquePtr<u8char[]>&& data, int32 length);

			~ContentData();

			const u8char* data = nullptr;

			/// @brief NULL included.
			int length = 0;

			uint32 Reference() const;
			uint32 Dereference() const;
			uint32 GetReferenceCount() const;

			/// @brief Get the global empty content data.
			static ReferencePtr<ContentData> GetEmpty();
		private:
			bool staticData;
			mutable ReferenceCount referenceCount;
		};

		class SearcherSunday {
		public:
			int32 Search(const u8char* target, int32 lenTarget, const u8char* pattern, int32 lenPattern);
		private:
			int32 charPos[256] = { -1 };
		};

		/// @brief Get the global empty String.
		static String GetEmpty();

#pragma region ctor/assignment
		/// @brief Creates a String from a raw C-Style string.
		/// Will make a copy of the content.
		/// @param string The C-Style string.
		/// @param count The char count. NULL NOT included. -1 for auto detection.
		String(const u8char* string = u8"", int32 count = -1);
		
		String& operator=(const u8char* string);

		String(const std::string& string);
		String(const std::u8string& string);

		/// @brief Creates a String using a pre-allocated ContentData.
		/// Will not do any copy.
		/// @param dataPtr The pre-allocated ContentData.
		/// @param start The start index of referencing content.
		/// @param count The char count for referencing content. NULL NOT included. -1 for auto detection.
		String(ReferencePtr<ContentData> dataPtr, int32 start = 0, int32 count = -1);
#pragma endregion

#pragma region Tool functions
		/// @brief Get char count. NULL NOT included.
		int32 GetCount() const;

		/// @brief Get the internal C-Style string array pointer. Do not store the pointer.
		const u8char* GetRawArray() const;

		/// @brief Check if the string is a individual one.
		/// Individual string means that the content of this string is exactally the same as the underlying raw string array.
		bool IsIndividual() const;
		
		/// @return A individual string whose content is the same as current string.
		/// If current string is already individual, return self.
		String ToIndividual() const;

		/// @brief Get the char at the given index.
		ReadonlyIterator<u8char> operator[](int32 index) const;

		/// @brief FindFind the position of the substring appearance in the string.
		/// @param pattern The substring to search.
		/// @param startFrom The index to start searching from.
		/// @param count The count of chars from the start index to search.
		/// -1 to search to the end of the string.
		/// @return The position of the substring occuring in the string.
		/// -1 if not found.
		int32 IndexOf(const String& pattern,int32 startFrom=0,int32 count=-1) const;

		/// @brief Check if the string contains another string.
		/// @param pattern The substring to search.
		bool Contains(const String& pattern) const;

		/// @brief Check if the string starts with another string.
		/// @param pattern The substring to search.
		bool StartsWith(const String& pattern) const;

		/// @brief Check if the string ends with another string.
		/// @param pattern The substring to search.
		bool EndsWith(const String& pattern) const;

		/// @param startIndex The index to start cutting from.
		/// @param count The count of chars from the start index to cut.
		String Substring(int32 startIndex, int32 count) const;

		/// @brief Replace a substring to another.
		/// @param from The substring to be replaced.
		/// @param to The string to be replaced to.
		String Replace(const String& from, const String& to) const;

		String ToString() const;
		int32 GetHashCode() const;

		int32 GetStartIndex() const;
		const u8char* GetStartPtr() const;

		bool operator==(const String& obj) const;
		bool operator!=(const String& obj) const;

		std::string_view GetStringView() const;
#pragma endregion

#pragma region Format
		template<typename ... Ts>
		static String Format(const String& format, const Ts& ... args) {
			return fmt::format(format.GetStringView(), args...);
		}
#pragma endregion

		String operator+(const String& obj);

	private:
		bool IsEqual(const String& obj) const;

		/// @brief Prepares a string, the string data will be copied.
		/// Count does not accept -1.
		void PrepareData(const u8char* string, sizeint count);

		ReferencePtr<ContentData> data;

		int32 refStart = 0;
		int32 refCount = 0;

		/// @brief Global sunday string searcher.
		static SearcherSunday searcher;

		static List<sizeint> replacerIndexes;
	};
}

namespace fmt {
	template<>
	struct formatter<::Engine::String> : formatter<string_view> {
		template <typename FormatContext>
		auto format(const ::Engine::String& c, FormatContext& ctx) {
			return formatter<string_view>::format(c.GetStringView(), ctx);
		}
	};
}