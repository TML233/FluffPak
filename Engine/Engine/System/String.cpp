#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/System/Exception.h"
#include "Engine/Collection/Iterator.h"
#include "Engine/System/IEnumerable.h"
#include <string>
#include <cstring>

namespace Engine {
	StringData::StringData(const char* data, int32 count) {
		length = count + 1;
		this->data = new char[length];
		// Copy data
		std::memcpy(this->data, data, count);
		// Add NULL
		std::memset(this->data + count, 0, sizeof(char));
	}
	StringData::~StringData() {
		delete[] data;
	}
	std::shared_ptr<StringData> StringData::empty = std::make_shared<StringData>("", sizeof(""));

	String::String(const char* string) {
		Prepare(string, std::strlen(string));
	}
	String::String(const std::string& string) {
		Prepare(string.c_str(), string.length());
	}
	String& String::operator=(const char* string) {
		Prepare(string, std::strlen(string));
		return *this;
	}

	ReadonlyIterator<char> String::operator[](int32 index) const {
		if (index >= data->length) {
			throw ArgumentOutOfRangeException("index", "out of the range of the string.");
		}
		return ReadonlyIterator<char>(&(data->data[index]));
	}

	int32 String::GetCount() const {
		return data->length - 1;
	}
	int32 String::GetLength() const {
		return data->length;
	}

	const char* String::GetRawArray() const {
		return data->data;
	}

	int32 String::IndexOf(String pattern, int32 from, int32 length) const {
		if (GetCount() < pattern.GetCount()) {
			return -1;
		}
		return searcher.Search(GetRawArray(), GetCount(), pattern.GetRawArray(), pattern.GetCount());
	}

	/*String String::Substring(int32 startIndex, int32 count) const {
		if (startIndex < 0) {
			throw ArgumentOutOfRangeException("startIndex", "Out of range.");
		}
		if ((count < 0) || count > (GetCount() - startIndex)) {
			throw ArgumentOutOfRangeException("count", "Out of range.");
		}
	}*/
	String String::ToString() const {
		return *this;
	}

	void String::Prepare(const char* string,int32 count) {
		// Use public empty string.
		if (count <= 0) {
			data = StringData::empty;
			return;
		}
		data = std::make_shared<StringData>(string,count);
	}

	String operator+(const String& left,const String& right) {
		return String::Format("{0}{1}", left, right);
	}

	StringSearcherSunday String::searcher{};
}