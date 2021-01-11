#ifndef HEADER_CORE_SYSTEM_STRING
#define HEADER_CORE_SYSTEM_STRING

#include "Core/System/Object.h"
#include <string>
#include "Thirdparty/fmt/include/fmt/core.h"
#include "Thirdparty/fmt/include/fmt/format.h"

namespace Core {
	template<typename T>
	class Iterator;
	template<typename T>
	class List;

	class String final :public Object {
	public:
		String(Char* string = "");
		String(const String& string);
		String(const std::string& string);
		String& operator=(const String& string);
		String& operator=(Char* string);
		~String();

		// Get byte length. NULL is not included.
		Int GetLength() const;

		// Get internal C-style char array.
		const Char* GetRawArray() const;

		Iterator<Char> operator[](Int index) const;

		template<typename ... Ts>
		static String Format(const String& format, const Ts& ... args);

		template<typename ... Ts>
		static String Format(const char* format, const Ts& ... args);

		String ToString() const override;

	private:
		friend String operator+(const String& left, const String& right);

		void ReplaceWithRawArray(const Char* string);

		List<Char>* chars;
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

#endif