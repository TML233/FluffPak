#include "Engine/System/String.h"
#include "Engine/System/Object.h"
#include "Engine/System/Debug.h"
#include "Engine/System/Memory.h"
#include "Engine/Collection/Iterator.h"
#include "Engine/System/UniquePtr.h"
#include <string>
#include <cstring>
#include <string_view>


namespace Engine {
	StringData::StringData(const char* data, int32 length, bool staticData):length(length),staticData(staticData) {
		if (!staticData) {
			UniquePtr<char[]> chars = UniquePtr<char[]>::Create(length);
			// Copy data and set null.
			std::memcpy(chars.GetRaw(), data, (sizeint)length - 1);
			std::memset(chars.GetRaw() + length - 1, 0, sizeof(char));
			this->data = chars.Release();
		} else {
			this->data = data;
		}
	}
	StringData::~StringData() {
		if (!staticData) {
			MEMDELARR(data);
		}
	}
	uint32 StringData::Reference() {
		if (staticData) {
			return 10;
		}
		return referenceCount.Reference();
	}
	uint32 StringData::Dereference() {
		if (staticData) {
			return 10;
		}
		return referenceCount.Dereference();
	}
	uint32 StringData::GetReferenceCount() const {
		if (staticData) {
			return 10;
		}
		return referenceCount.Get();
	}
	
	StringData StringData::empty = StringData("", sizeof(""), true);

	String::String(const char* string) {
		PrepareData(string, static_cast<int32>(std::strlen(string)));
	}
	String::String(const std::string& string) {
		PrepareData(string.c_str(), static_cast<int32>(string.length()));
	}
	String& String::operator=(const char* string) {
		PrepareData(string, static_cast<int32>(std::strlen(string)));
		return *this;
	}

	String::String(ReferencePtr<StringData> dataPtr) :data(dataPtr), refStart(0), refCount(dataPtr->length - 1) {}

	bool String::IsIndividual() const {
		return (refStart == 0 && refCount == data->length - 1);
	}

	String String::ToIndividual() const {
		if (IsIndividual()) {
			return *this;
		}
		return String(data->data + refStart, refCount);
	}

	ReadonlyIterator<char> String::operator[](int32 index) const {
		ERR_ASSERT_ACTION(index >= 0 && index <= GetCount(), "index out of bounds.", return ReadonlyIterator<char>((char*)""));
		
		return ReadonlyIterator<char>(data->data + refStart + index);
	}

	int32 String::GetCount() const {
		return refCount;
	}

	const char* String::GetRawArray() const {
		return data->data;
	}

	int32 String::IndexOf(const String& pattern,int32 startFrom,int32 count) const {
		ERR_ASSERT_ACTION(startFrom >= 0 && startFrom < GetCount(), "startFrom out of bounds.", return -1);
		ERR_ASSERT_ACTION(count >= -1 && count <= (GetCount() - startFrom), "count out of bounds.", return -1);
		
		if (GetCount() < pattern.GetCount()) {
			return -1;
		}
		if (count == 0) {
			return -1;
		}
		if (count == -1) {
			count = GetCount() - startFrom;
		}
		return searcher.Search(GetStartPtr()+startFrom, count, pattern.GetStartPtr(), pattern.GetCount());
	}

	bool String::Contains(const String& pattern) const {
		return IndexOf(pattern) >= 0;
	}

	String String::Substring(int32 startIndex, int32 count) const {
		ERR_ASSERT_ACTION(startIndex >= 0 && startIndex < GetCount(), "startIndex out of bounds.", return String());
		ERR_ASSERT_ACTION(count >= 0 && count <= (GetCount() - startIndex), "count out of bounds.", return String());

		String substr = *this;
		substr.refStart = startIndex;
		substr.refCount = count;
		return substr;
	}

	bool String::StartsWith(const String& pattern) const {
		if (GetCount() < pattern.GetCount()) {
			return false;
		}
		return (Substring(0, pattern.GetCount()) == pattern);
	}
	bool String::EndsWith(const String& pattern) const {
		if (GetCount() < pattern.GetCount()) {
			return false;
		}
		return (Substring(GetCount() - pattern.GetCount(), pattern.GetCount()) == pattern);
	}

	bool String::IsEqual(const String& obj) const {
		if (GetCount() != obj.GetCount()) {
			return false;
		}

		if (data.GetRaw() == obj.data.GetRaw() && refStart == obj.refStart) {
			return true;
		}

		if (GetHashCode() != obj.GetHashCode()) {
			return false;
		}

		const char* ptrA = GetStartPtr();
		const char* ptrB = obj.GetStartPtr();
		for (int i = 0; i < GetCount(); i += 1, ptrA += 1, ptrB += 1) {
			if (*ptrA != *ptrB) {
				return false;
			}
		}

		return true;
	}

	bool String::operator==(const String& obj) const {
		return IsEqual(obj);
	}
	bool String::operator!=(const String& obj) const {
		return !IsEqual(obj);
	}

	String String::ToString() const {
		return *this;
	}
	int32 String::GetHashCode() const {
		return Object::GetHashCode(std::hash<std::string_view>{}(std::string_view(GetStartPtr(), GetCount())));
	}

	String::String(const char* string, int32 count) {
		PrepareData(string, count);
	}

	int32 String::GetStartIndex() const {
		return refStart;
	}
	const char* String::GetStartPtr() const {
		return data->data + refStart;
	}

	void String::PrepareData(const char* string,int32 count) {
		// Use public empty string.
		if (count <= 0) {
			data = ReferencePtr<StringData>(&StringData::empty);
			return;
		}
		data = ReferencePtr<StringData>::Create(string, count + 1);
		refStart = 0;
		refCount = count;
	}

	String operator+(const String& left,const String& right) {
		return String::Format("{0}{1}", left, right);
	}

	StringSearcherSunday String::searcher{};
}