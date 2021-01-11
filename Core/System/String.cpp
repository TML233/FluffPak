#include "Core/Definition.h"
#include "Core/System/String.h"
#include "Core/Template/List.h"

#include <cstring>

namespace Core {
	void String::ReplaceWithRawArray(const Char* string) {
		Int size = std::strlen(string) + 1;
		chars->Clear();
		if (chars->GetCapacity() < size) {
			chars->SetCapacity(size);
		}
		for (Int i = 0; i < size; i += 1) {
			chars->Add(string[i]);
		}
	}

	String::String(Char* string) {
		chars = new List<Char>;
		ReplaceWithRawArray(string);
	}
	String::String(const String& string) {
		chars = new List<Char>;
		ReplaceWithRawArray(string.GetRawArray());
	}
	String::String(const std::string& string) {
		chars = new List<Char>;
		ReplaceWithRawArray(string.c_str());
	}

	String::~String() {
		delete chars;
	}

	String& String::operator=(const String& string) {
		ReplaceWithRawArray(string.GetRawArray());
		return *this;
	}
	String& String::operator=(Char* string) {
		ReplaceWithRawArray(string);
		return *this;
	}

	Iterator<Char> String::operator[](Int index) const {
		return Iterator<Char>((*chars)[index]);
	}

	Int String::GetLength() const {
		return chars->GetCount() - 1;
	}

	const Char* String::GetRawArray() const {
		return chars->GetRawArray();
	}

	String String::ToString() const {
		return *this;
	}

	String operator+(const String& left,const String& right) {
		String str = left;
		str.chars->PrepareCapacity(left.GetLength() + right.GetLength() + 1);
		str.chars->RemoveAt(str.chars->GetCount() - 1);
		for (Char& c : *(right.chars)) {
			str.chars->Add(c);
		}
		return str;
	}
}