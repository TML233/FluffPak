#ifndef HEADER_CORE_SYSTEM_STRING
#define HEADER_CORE_SYSTEM_STRING

#include "Engine/System/Object.h"
#include <memory>
#include "fmt/core.h"
#include "fmt/format.h"

namespace Engine {
	template<typename T>
	class ReadonlyIterator;
	template<typename T>
	class List;

	struct StringData final {
		explicit StringData(const Char* data);
		~StringData();

		Char* data = nullptr;
		int length = 0;

		static std::shared_ptr<StringData> empty;
	};

	class String final :public Object {
	public:
		String(const Char* string = "");
		String(const std::string& string);
		String& operator=(const Char* string);

		// Get byte length. NULL is not included.
		Int GetLength() const;

		// Get internal C-style char array.
		const Char* GetRawArray() const;

		ReadonlyIterator<Char> operator[](Int index) const;

		template<typename ... Ts>
		static String Format(const String& format, const Ts& ... args);
		template<typename ... Ts>
		static String Format(const Char* format, const Ts& ... args);

		String ToString() const override;

	private:
		friend String operator+(const String& left, const String& right);
		void Prepare(const Char* string);
		std::shared_ptr<StringData> data;
	};

	template<typename ... Ts>
	static String String::Format(const String& format, const Ts& ... args) {
		return fmt::format(format.GetRawArray(), args...);
	}

	template<typename ... Ts>
	static String String::Format(const Char* format, const Ts& ... args) {
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