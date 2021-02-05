#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/Collection/Iterator.h"
#include <string>
#include <cstring>

namespace Engine {
	StringData::StringData(const char* data) {
		int32 len = strlen(data);
		length = len;

		len += 1;
		this->data = new char[len];
		for (int32 i = 0; i < len; i += 1) {
			this->data[i] = data[i];
		}
	}
	StringData::~StringData() {
		delete[] data;
	}
	std::shared_ptr<StringData> StringData::empty = std::make_shared<StringData>("");

	String::String(const char* string) {
		Prepare(string);
	}
	String::String(const std::string& string) {
		Prepare(string.c_str());
	}
	String& String::operator=(const char* string) {
		Prepare(string);
		return *this;
	}

	ReadonlyIterator<char> String::operator[](int32 index) const {
		return ReadonlyIterator<char>(&(data->data[index]));
	}

	int32 String::GetLength() const {
		return data->length;
	}

	const char* String::GetRawArray() const {
		return data->data;
	}

	int32 String::IndexOf(String pattern, int32 from, int32 length) const {
		if (GetLength() < pattern.GetLength()) {
			return -1;
		}
		return searcher.Search(GetRawArray(), GetLength(), pattern.GetRawArray(), pattern.GetLength());
	}

	String String::ToString() const {
		return *this;
	}

	void String::Prepare(const char* string) {
		int32 len = strlen(string);
		// Use public empty string.
		if (len <= 0) {
			data = StringData::empty;
			return;
		}
		data = std::make_shared<StringData>(string);
	}

	String operator+(const String& left,const String& right) {
		return String::Format("{0}{1}", left, right);
	}

	StringSearcherSunday String::searcher{};
}