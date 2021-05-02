#include "Engine/System/String.h"
#include "Engine/System/ObjectUtil.h"
#include "Engine/System/Debug.h"
#include "Engine/System/Memory.h"
#include "Engine/Collection/Iterator.h"
#include "Engine/System/UniquePtr.h"
#include <string>
#include <cstring>
#include <string_view>


namespace Engine {
#pragma region ContentData
	String::ContentData::ContentData(const u8char* data, int32 length) :data(data), length(length), staticData(true) {}
	String::ContentData::ContentData(UniquePtr<u8char[]>&& data, int32 length) : data(data.Release()), length(length), staticData(false) {}

	String::ContentData::~ContentData() {
		if (!staticData) {
			MEMDELARR(data);
		}
	}
	uint32 String::ContentData::Reference() const {
		if (staticData) {
			return 1;
		}
		return referenceCount.Reference();
	}
	uint32 String::ContentData::Dereference() const {
		if (staticData) {
			return 1;
		}
		return referenceCount.Dereference();
	}
	uint32 String::ContentData::GetReferenceCount() const {
		if (staticData) {
			return 1;
		}
		return referenceCount.Get();
	}
	
	ReferencePtr<String::ContentData> String::ContentData::GetEmpty() {
		static const ContentData empty(u8"", 1);
		static ReferencePtr<ContentData> ptr{ const_cast<ContentData*>(&empty) };
		return ptr;
	}
#pragma endregion

	int32 String::SearcherSunday::Search(const u8char* target, int32 lenTarget, const u8char* pattern, int32 lenPattern) {
		//Prepare charPos
		std::memset(charPos, -1, 256 * sizeof(int32));
		for (int32 i = 0; i < lenPattern; i += 1) {
			byte index = 0;
			memcpy(&index, pattern + i, sizeof(byte));
			charPos[index] = i;
		}

		int32 pos = 0;
		for (int32 i = 0; i < lenPattern; i += 1) {
			if (pos > lenTarget - 1) {
				// Not found
				return -1;
			}
			if (target[pos + i] != pattern[i]) {
				int32 checkpos = pos + lenPattern;
				if (checkpos > lenTarget - 1) {
					// Not found
					return -1;
				} else {
					byte checking = 0;
					std::memcpy(&checking, target + checkpos, sizeof(byte));
					int32 cpos = charPos[checking];
					if (cpos < 0) {
						pos += lenPattern + 1;
					} else {
						pos += checkpos - (pos + cpos);
					}
					i = -1;
				}
			}
		}
		return pos;
	}


	String String::GetEmpty() {
		return String(ReferencePtr<ContentData>(ContentData::GetEmpty()));
	}

	String::String(const u8char* string,int32 count) {
		if (count < 0) {
			count = static_cast<int32>(std::strlen(reinterpret_cast<const char*>(string)));
		}
		PrepareData(string, count);
	}
	String& String::operator=(const u8char* string) {
		PrepareData(string, static_cast<int32>(std::strlen(reinterpret_cast<const char*>(string))));
		return *this;
	}

	String::String(const std::string& string) {
		PrepareData(reinterpret_cast<const u8char*>(string.c_str()), static_cast<int32>(string.length()));
	}
	String::String(const std::u8string& string) {
		PrepareData(string.c_str(), static_cast<int32>(string.length()));
	}

	String::String(ReferencePtr<ContentData> dataPtr, int32 start, int32 count) :data(dataPtr), refStart(start), refCount(count < 0 ? dataPtr->length - 1 : count) {}

	void String::PrepareData(const u8char* string, sizeint count) {
		// Use public empty string.
		if (count <= 0) {
			data = ContentData::GetEmpty();
			refStart = 0;
			refCount = 0;
			return;
		}

		sizeint len = count + 1;
		UniquePtr<u8char[]> strData = UniquePtr<u8char[]>::Create(len);
		std::memcpy(strData.GetRaw(), string, count);
		std::memset(strData.GetRaw() + len-1, '\0', 1);

		data = ReferencePtr<ContentData>::Create(Memory::Move(strData), len);
		refStart = 0;
		refCount = count;
	}

	bool String::IsIndividual() const {
		return (refStart == 0 && refCount == data->length - 1);
	}

	String String::ToIndividual() const {
		if (IsIndividual()) {
			return *this;
		}
		return String(data->data + refStart, refCount);
	}

	ReadonlyIterator<u8char> String::operator[](int32 index) const {
		ERR_ASSERT(index >= 0 && index <= GetCount(), u8"index out of bounds.", return ReadonlyIterator<u8char>(const_cast<u8char*>(u8"")));
		
		return ReadonlyIterator<u8char>(data->data + refStart + index);
	}

	int32 String::GetCount() const {
		return refCount;
	}

	const u8char* String::GetRawArray() const {
		return data->data;
	}

	int32 String::IndexOf(const String& pattern,int32 startFrom,int32 count) const {
		ERR_ASSERT(startFrom >= 0 && startFrom < GetCount(), u8"startFrom out of bounds.", return -1);
		ERR_ASSERT(count >= -1 && count <= (GetCount() - startFrom), u8"count out of bounds.", return -1);
		
		if (GetCount() < pattern.GetCount()) {
			return -1;
		}
		if (count == 0) {
			return -1;
		}
		if (count == -1) {
			count = GetCount() - startFrom;
		}
		int32 found=searcher.Search(GetStartPtr()+startFrom, count, pattern.GetStartPtr(), pattern.GetCount());
		return (found < 0 ? -1 : startFrom + found);
	}

	bool String::Contains(const String& pattern) const {
		return IndexOf(pattern) >= 0;
	}

	String String::Substring(int32 startIndex, int32 count) const {
		ERR_ASSERT(startIndex >= 0 && startIndex < GetCount(), u8"startIndex out of bounds.", return String());
		ERR_ASSERT(count >= 0 && count <= (GetCount() - startIndex), u8"count out of bounds.", return String());

		String substr = *this;
		substr.refStart = startIndex;
		substr.refCount = count;
		return substr;
	}

	List<sizeint> String::replacerIndexes{ 32 };

	String String::Replace(const String& from, const String& to) const {
		replacerIndexes.Clear();

		// Search for appearence times.
		{
			int start = 0;
			while (start < GetCount()) {
				int index = IndexOf(from, start);
				if (index >= 0) {
					replacerIndexes.Add(index);
					start = index + from.GetCount();
				} else {
					break;
				}
			}
		}

		int times = replacerIndexes.GetCount();
		if (times <= 0) {
			return *this;
		}

		sizeint rawlen = GetCount() + (-from.GetCount() + to.GetCount()) * times + 1;
		UniquePtr<u8char[]> rawptr = UniquePtr<u8char[]>::Create(rawlen);
		u8char* raw = rawptr.GetRaw();

		// Fill the string.
		sizeint rawi = 0;
		for (int i = 0; i < times; i += 1) {
			sizeint start = (i == 0 ? 0 : replacerIndexes.Get(i - 1)+from.GetCount());
			sizeint end = replacerIndexes.Get(i);
			sizeint len = end - start;
			std::memcpy(raw + rawi, GetStartPtr() + start, len);
			rawi += len;

			std::memcpy(raw + rawi, to.GetStartPtr(), to.GetCount());
			rawi += to.GetCount();
		}
		sizeint end = replacerIndexes.Get(times-1);
		std::memcpy(raw + rawi, GetStartPtr() + (end + 1), GetCount() - (end + 1));
		std::memset(raw + rawlen - 1, '\0', 1);

		return String(ReferencePtr<ContentData>::Create(Memory::Move(rawptr), rawlen));
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

		const u8char* ptrA = GetStartPtr();
		const u8char* ptrB = obj.GetStartPtr();
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
		return ObjectUtil::GetHashCode(std::hash<std::string_view>{}(GetStringView()));
	}

	int32 String::GetStartIndex() const {
		return refStart;
	}
	const u8char* String::GetStartPtr() const {
		return data->data + refStart;
	}

	String String::operator+(const String& obj) {
		return String::Format(u8"{0}{1}", *this, obj);
	}

	std::string_view String::GetStringView() const {
		return std::string_view(reinterpret_cast<const char*>(GetStartPtr()), GetCount());
	}

	String::SearcherSunday String::searcher{};
}