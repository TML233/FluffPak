#ifndef HEADER_CORE_SYSTEM_STRING
#define HEADER_CORE_SYSTEM_STRING

#include "Engine/System/Object.h"
#include <memory>
#include "fmt/core.h"
#include "fmt/format.h"
#include "Engine/Algorithm/StringSearcherSunday.h"

namespace Engine {
	template<typename T>
	class ReadonlyIterator;
	template<typename T>
	class List;

	struct StringData final {
		explicit StringData(const char* data);
		~StringData();

		// Original data of the string.
		char* data = nullptr;
		// NULL not included.
		int length = 0;

		static std::shared_ptr<StringData> empty;
	};

	class String final :public Object {
	public:
		String(const char* string = "");
		String(const std::string& string);
		String& operator=(const char* string);

		// Get byte length. NULL not included.
		int32 GetLength() const;

		// Get internal C-style char array.
		// Do not store the pointer.
		const char* GetRawArray() const;

		ReadonlyIterator<char> operator[](int32 index) const;

		// Find the position of the substring appearance in the current string.
		// Return -1 if not found.
		int32 IndexOf(String pattern,int32 from=0, int32 length=-1) const;

		bool StartsWith(String pattern) const;
		bool EndsWith(String pattern) const;


		template<typename ... Ts>
		static String Format(const String& format, const Ts& ... args);
		template<typename ... Ts>
		static String Format(const char* format, const Ts& ... args);

		String ToString() const override;

	private:
		void Prepare(const char* string);
		std::shared_ptr<StringData> data;

		static StringSearcherSunday searcher;
	};



	template<typename ... Ts>
	static String String::Format(const String& format, const Ts& ... args) {
		return fmt::format(format.GetRawArray(), args...);
	}

	template<typename ... Ts>
	static String String::Format(const char* format, const Ts& ... args) {
		return fmt::format(format, args...);
	}

	String operator+(const String& left, const String& right);
}

using namespace Engine;
namespace fmt {
	template<>
	struct formatter<String> : formatter<string_view> {
		template <typename FormatContext>
		auto format(const String& c, FormatContext& ctx) {
			return formatter<string_view>::format(c.GetRawArray(), ctx);
		}
	};
}

#endif