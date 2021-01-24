#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/Collection/Iterator.h"
#include <string>
#include <cstring>

namespace Engine {
	StringData::StringData(const Char* data) {
		Int len = strlen(data) + 1;
		length = len;
		this->data = new Char[len];
		for (Int i = 0; i < len; i += 1) {
			this->data[i] = data[i];
		}
	}
	StringData::~StringData() {
		delete[] data;
	}
	std::shared_ptr<StringData> StringData::empty = std::make_shared<StringData>("");

	String::String(const Char* string) {
		Prepare(string);
	}
	String::String(const std::string& string) {
		Prepare(string.c_str());
	}
	String& String::operator=(const Char* string) {
		Prepare(string);
		return *this;
	}

	ReadonlyIterator<Char> String::operator[](Int index) const {
		return ReadonlyIterator<Char>(&(data->data[index]));
	}

	Int String::GetLength() const {
		return data->length - 1;
	}

	const Char* String::GetRawArray() const {
		return data->data;
	}

	String String::ToString() const {
		return *this;
	}

	void String::Prepare(const Char* string) {
		Int len = strlen(string) + 1;
		// Use public empty string.
		if (len <= 1) {
			data = StringData::empty;
			return;
		}
		data = std::make_shared<StringData>(string);
	}

	String operator+(const String& left,const String& right) {
		return String::Format("{0}{1}", left, right);
	}
}