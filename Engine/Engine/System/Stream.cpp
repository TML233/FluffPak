#include "Engine/System/Stream.h"

namespace Engine {
	Stream::Endianness Stream::GetCurrentEndianness() const {
		return currentEndianness;
	}
	void Stream::SetCurrentEndianness(Stream::Endianness endianness) {
		currentEndianness = endianness;
	}

	void Stream::SwapBuffer(byte* ptr, int32 length) {
		for (int i = 0; i < length / 2; i += 1) {
			byte temp = *(ptr + i);
			*(ptr + i) = *(ptr + length - 1 - i);
			*(ptr + length - 1 - i) = temp;
		}
	}

	ResultCode Stream::WriteBytes(const byte* valuePtr, int32 length) {
		ERR_ASSERT(IsValid(), u8"Attempted to operate an invalid FileStream!", return ResultCode::InvalidStream);
		ERR_ASSERT(CanWrite(), u8"This FileStream cannot write.", return ResultCode::NoPermission);
		ERR_ASSERT(length >= 0, u8"length must be greater than 0.", return ResultCode::InvalidArgument);
		if (length == 0) {
			return ResultCode::OK;
		}
		return WriteBytesUnchecked(valuePtr, length);
	}
	ResultCode Stream::WriteBytesEndian(const byte* valuePtr, int32 length) {
		ERR_ASSERT(IsValid(), u8"Attempted to operate an invalid FileStream!", return ResultCode::InvalidStream);
		ERR_ASSERT(CanWrite(), u8"This FileStream cannot write.", return ResultCode::NoPermission);
		ERR_ASSERT(length >= 0, u8"length must be greater than 0.", return ResultCode::InvalidArgument);
		if (length == 0) {
			return ResultCode::OK;
		}

		if (Stream::LocalEndianness == GetCurrentEndianness()) {
			return WriteBytesUnchecked(valuePtr, length);
		}

		rwCache.Clear();
		for (int32 i = 0; i < length; i += 1) {
			rwCache.Add(*(valuePtr + length - 1 - i));
		}
		return WriteBytesUnchecked(rwCache.GetRawElementPtr(), length);
	}
	
	ResultCode Stream::WriteByte(byte value) {
		return WriteBytes(&value, sizeof(byte));
	}
	ResultCode Stream::WriteBytes(const List<byte>& value,int32 startIndex,int32 length) {
		int32 count = value.GetCount();
		ERR_ASSERT(startIndex >= 0 && startIndex < count, u8"startIndex out of bounds!", return ResultCode::InvalidArgument);
		ERR_ASSERT(length >= -1 && length <= count - startIndex, u8"length out of bounds!", return ResultCode::InvalidArgument);
		if (count == -1) {
			length = count - startIndex;
		}
		if (length == 0) {
			return ResultCode::OK;
		}
		return WriteBytes(value.GetRawElementPtr() + startIndex, length);
	}
	ResultCode Stream::WriteSByte(sbyte value) {
		return WriteBytes((byte*)&value, sizeof(sbyte));
	}
	ResultCode Stream::WriteInt16(int16 value) {
		return WriteBytesEndian((byte*)&value, sizeof(int16));
	}
	ResultCode Stream::WriteUInt16(uint16 value) {
		return WriteBytesEndian((byte*)&value, sizeof(uint16));
	}
	ResultCode Stream::WriteInt32(int32 value) {
		return WriteBytesEndian((byte*)&value, sizeof(int32));
	}
	ResultCode Stream::WriteUInt32(uint32 value) {
		return WriteBytesEndian((byte*)&value, sizeof(uint32));
	}
	ResultCode Stream::WriteInt64(int64 value) {
		return WriteBytesEndian((byte*)&value, sizeof(int64));
	}
	ResultCode Stream::WriteUInt64(uint64 value) {
		return WriteBytesEndian((byte*)&value, sizeof(uint64));
	}
	ResultCode Stream::WriteFloat(float value) {
		return WriteBytesEndian((byte*)&value, sizeof(float));
	}
	ResultCode Stream::WriteDouble(double value) {
		return WriteBytesEndian((byte*)&value, sizeof(double));
	}
	ResultCode Stream::WriteString(const String& value) {
		Write7BitEncodedInt(value.GetCount());
		return WriteText(value);
	}
	ResultCode Stream::WriteText(const String& value) {
		return WriteBytes((byte*)value.GetStartPtr(), value.GetCount());
	}
	ResultCode Stream::WriteTextLine(const String& value) {
		auto result = WriteText(value);
		WriteByte((byte)'\n');
		return result;
	}
	ResultCode Stream::Write7BitEncodedInt(int32 value) {
		// Implementation from .NET Source
		// See https://source.dot.net/#System.Private.CoreLib/BinaryWriter.cs,2daa1d14ff1877bd
		rwCache.Clear();
		uint32 uValue = (uint32)value;
		while (uValue > 0x7Fu) {
			rwCache.Add((byte)(uValue | ~0x7Fu));
			uValue >>= 7;
		}
		rwCache.Add((byte)uValue);
		return WriteBytes(rwCache);
	}
	ResultCode Stream::TryReadBytes(int32 length, int32& readCount, List<byte>& result) {
		readCount = 0;
		ERR_ASSERT(length >= 0, u8"length must be greater than 0.", return ResultCode::InvalidArgument);
		if (length == 0) {
			return ResultCode::OK;
		}

		//Prepare result container
		result.EnsureCapacity(result.GetCapacity() + length);
		for (int32 i = 0; i < length; i += 1) {
			result.Add(0);
		}

		auto resultCode = TryReadBytesUnchecked(length, readCount, result);

		if (resultCode != ResultCode::OK) {
			ERR_MSG(u8"TryReadBytesUnchecked failed!");
			readCount = 0;
		}

		int32 remove = length - readCount;
		for (int32 i = 0; i < remove; i += 1) {
			result.RemoveAt(result.GetCount() - 1 - i);
		}

		return resultCode;
	}
	ResultCode Stream::TryReadBytesEndian(int32 length, int32& readCount, List<byte>& result) {
		auto resultCode = TryReadBytes(length, readCount, result);
		if (readCount <= 0) {
			return ResultCode::OK;
		}

		// Swap bytes if needed.
		if (Stream::LocalEndianness != GetCurrentEndianness()) {
			SwapBuffer(result.GetRawElementPtr() + result.GetCount() - readCount, readCount);
		}

		return ResultCode::OK;
	}

	/*
	int32 Stream::ReadBytes(int32 length, List<byte>& result) {
		int32 readCount = 0;
		auto resultCode = TryReadBytes(length, readCount, result);
		ERR_ASSERT(resultCode == ResultCode::OK, u8"TryReadBytesDirectly failed!", return 0);
		return readCount;
	}
	int32 Stream::ReadBytesEndian(int32 length, List<byte>& result) {
		int32 readCount = 0;
		auto resultCode = TryReadBytesEndian(length, readCount, result);
		ERR_ASSERT(resultCode == ResultCode::OK, u8"TryReadBytes failed!", return 0);
		return readCount;
	}
	byte Stream::ReadByte() {
		rwCache.Clear();
		int32 read = ReadBytes(sizeof(byte), rwCache);
		ERR_ASSERT(read == sizeof(byte), u8"Cannot read a byte.", return 0);
		return *((byte*)(rwCache.GetRawElementPtr() + rwCache.GetCount() - read));
	}
	sbyte Stream::ReadSByte() {
		rwCache.Clear();
		int32 read = ReadBytes(sizeof(sbyte), rwCache);
		ERR_ASSERT(read == sizeof(sbyte), u8"Cannot read a sbyte.", return 0);
		return *((sbyte*)(rwCache.GetRawElementPtr() + rwCache.GetCount() - read));
	}
	int16 Stream::ReadInt16() {
		rwCache.Clear();
		int32 read = ReadBytesEndian(sizeof(int16), rwCache);
		ERR_ASSERT(read == sizeof(int16), u8"Cannot read a int16.", return 0);
		return *((int16*)(rwCache.GetRawElementPtr() + rwCache.GetCount() - read));
	}
	uint16 Stream::ReadUInt16() {
		rwCache.Clear();
		int32 read = ReadBytesEndian(sizeof(uint16), rwCache);
		ERR_ASSERT(read == sizeof(uint16), u8"Cannot read a uint16.", return 0);
		return *((uint16*)(rwCache.GetRawElementPtr() + rwCache.GetCount() - read));
	}
	int32 Stream::ReadInt32() {
		rwCache.Clear();
		int32 read = ReadBytesEndian(sizeof(int32), rwCache);
		ERR_ASSERT(read == sizeof(int32), u8"Cannot read a int32.", return 0);
		return *((int32*)(rwCache.GetRawElementPtr() + rwCache.GetCount() - read));
	}
	uint32 Stream::ReadUInt32() {
		rwCache.Clear();
		int32 read = ReadBytesEndian(sizeof(uint32), rwCache);
		ERR_ASSERT(read == sizeof(uint32), u8"Cannot read a uint32.", return 0);
		return *((uint32*)(rwCache.GetRawElementPtr() + rwCache.GetCount() - read));
	}
	int64 Stream::ReadInt64() {
		rwCache.Clear();
		int32 read = ReadBytesEndian(sizeof(int64), rwCache);
		ERR_ASSERT(read == sizeof(int64), u8"Cannot read a int64.", return 0);
		return *((int64*)(rwCache.GetRawElementPtr() + rwCache.GetCount() - read));
	}
	uint64 Stream::ReadUInt64() {
		rwCache.Clear();
		int32 read = ReadBytesEndian(sizeof(uint64), rwCache);
		ERR_ASSERT(read == sizeof(uint64), u8"Cannot read a uint64.", return 0);
		return *((uint64*)(rwCache.GetRawElementPtr() + rwCache.GetCount() - read));
	}
	float Stream::ReadFloat() {
		rwCache.Clear();
		int32 read = ReadBytesEndian(sizeof(float), rwCache);
		ERR_ASSERT(read == sizeof(float), u8"Cannot read a float.", return 0);
		return *((float*)(rwCache.GetRawElementPtr() + rwCache.GetCount() - read));
	}
	double Stream::ReadDouble() {
		rwCache.Clear();
		int32 read = ReadBytesEndian(sizeof(double), rwCache);
		ERR_ASSERT(read == sizeof(double), u8"Cannot read a double.", return 0);
		return *((double*)(rwCache.GetRawElementPtr() + rwCache.GetCount() - read));
	}
	// TODO: Implement this.
	String Stream::ReadString() {
		FATAL_CRASH(u8"Not implemented.");
		return String::GetEmpty();
	}

	// TODO: Implement this.
	String Stream::ReadTextLine() {
		FATAL_CRASH(u8"Not implemented.");
		return String::GetEmpty();
	}
	
	String Stream::ReadAllText() {
		ERR_ASSERT(CanRandomAccess(), u8"Stream must be capable with random accessing!", return String::GetEmpty());

		SetPosition(0);
		int32 length = GetLength();
		
		rwCache.Clear();
		rwCache.EnsureCapacity(length + 1);

		int32 read=ReadBytes(length, rwCache);
		ERR_ASSERT(read == length, u8"Error when read bytes!", return String::GetEmpty());

		return String((u8char*)rwCache.GetRawElementPtr(), length);
	}
	*/
}
