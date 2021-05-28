#include "Engine/System/Stream.h"

namespace Engine {
	Stream::Endianness Stream::GetCurrentEndianness() const {
		return currentEndianness;
	}
	void Stream::SetCurrentEndianness(Stream::Endianness endianness) {
		currentEndianness = endianness;
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

		ResultCode r = ResultCode::OK;
		for (int32 i = length - 1; i >= 0 && r == ResultCode::OK; i -= 1) {
			r = WriteBytesUnchecked(valuePtr + i, 1);
		}
		return r;
	}
	int32 Stream::ReadBytes(int32 length, List<byte>& result) {
		ERR_ASSERT(length >= 0, u8"length must be greater than 0.", return 0);
		if (length == 0) {
			return 0;
		}

		//Prepare result container
		result.RequireCapacity(result.GetCapacity() + length);
		for (int32 i = 0; i < length; i += 1) {
			result.Add(0);
		}

		int32 read = ReadBytesUnchecked(length, result);

		int32 remove = length - read;
		for (int32 i = 0; i < remove; i += 1) {
			result.RemoveAt(result.GetCount() - 1 - i);
		}

		return read;
	}
	int32 Stream::ReadBytesEndian(int32 length, List<byte>& result) {
		int32 read = ReadBytes(length, result);
		if (read <= 0) {
			return 0;
		}

		// Swap bytes if needed.
		if (Stream::LocalEndianness != GetCurrentEndianness()) {
			auto ptr = result.GetRawElementPtr() + result.GetCount() - read;
			for (int i = 0; i < read / 2; i += 1) {
				byte temp = *(ptr + i);
				*(ptr + i) = *(ptr + read - 1 - i);
				*(ptr + read - 1 - i) = temp;
			}
		}

		return read;
	}

	ResultCode Stream::WriteByte(byte value) {
		return WriteBytes(&value, sizeof(byte));
	}
	ResultCode Stream::WriteBytes(const List<byte>& value) {
		return WriteBytes(value.begin().GetPointer(), value.GetCount());
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
	// TODO: Implement this.
	ResultCode Stream::WriteString(const String& value) {
		FATAL_CRASH(u8"Not implemented.");
		return ResultCode::NotSupported;
	}
	ResultCode Stream::WriteText(const String& value) {
		return WriteBytes((byte*)value.GetStartPtr(), value.GetCount());
	}
	ResultCode Stream::WriteTextLine(const String& value) {
		auto result = WriteText(value);
		WriteByte((byte)'\n');
		return result;
	}

	byte Stream::ReadByte() {
		readCache.Clear();
		int32 read = ReadBytes(sizeof(byte), readCache);
		ERR_ASSERT(read == sizeof(byte), u8"Cannot read a byte.", return 0);
		return *((byte*)(readCache.GetRawElementPtr() + readCache.GetCount() - read));
	}
	sbyte Stream::ReadSByte() {
		readCache.Clear();
		int32 read = ReadBytes(sizeof(sbyte), readCache);
		ERR_ASSERT(read == sizeof(sbyte), u8"Cannot read a sbyte.", return 0);
		return *((sbyte*)(readCache.GetRawElementPtr() + readCache.GetCount() - read));
	}
	int16 Stream::ReadInt16() {
		readCache.Clear();
		int32 read = ReadBytesEndian(sizeof(int16), readCache);
		ERR_ASSERT(read == sizeof(int16), u8"Cannot read a int16.", return 0);
		return *((int16*)(readCache.GetRawElementPtr() + readCache.GetCount() - read));
	}
	uint16 Stream::ReadUInt16() {
		readCache.Clear();
		int32 read = ReadBytesEndian(sizeof(uint16), readCache);
		ERR_ASSERT(read == sizeof(uint16), u8"Cannot read a uint16.", return 0);
		return *((uint16*)(readCache.GetRawElementPtr() + readCache.GetCount() - read));
	}
	int32 Stream::ReadInt32() {
		readCache.Clear();
		int32 read = ReadBytesEndian(sizeof(int32), readCache);
		ERR_ASSERT(read == sizeof(int32), u8"Cannot read a int32.", return 0);
		return *((int32*)(readCache.GetRawElementPtr() + readCache.GetCount() - read));
	}
	uint32 Stream::ReadUInt32() {
		readCache.Clear();
		int32 read = ReadBytesEndian(sizeof(uint32), readCache);
		ERR_ASSERT(read == sizeof(uint32), u8"Cannot read a uint32.", return 0);
		return *((uint32*)(readCache.GetRawElementPtr() + readCache.GetCount() - read));
	}
	int64 Stream::ReadInt64() {
		readCache.Clear();
		int32 read = ReadBytesEndian(sizeof(int64), readCache);
		ERR_ASSERT(read == sizeof(int64), u8"Cannot read a int64.", return 0);
		return *((int64*)(readCache.GetRawElementPtr() + readCache.GetCount() - read));
	}
	uint64 Stream::ReadUInt64() {
		readCache.Clear();
		int32 read = ReadBytesEndian(sizeof(uint64), readCache);
		ERR_ASSERT(read == sizeof(uint64), u8"Cannot read a uint64.", return 0);
		return *((uint64*)(readCache.GetRawElementPtr() + readCache.GetCount() - read));
	}
	float Stream::ReadFloat() {
		readCache.Clear();
		int32 read = ReadBytesEndian(sizeof(float), readCache);
		ERR_ASSERT(read == sizeof(float), u8"Cannot read a float.", return 0);
		return *((float*)(readCache.GetRawElementPtr() + readCache.GetCount() - read));
	}
	double Stream::ReadDouble() {
		readCache.Clear();
		int32 read = ReadBytesEndian(sizeof(double), readCache);
		ERR_ASSERT(read == sizeof(double), u8"Cannot read a double.", return 0);
		return *((double*)(readCache.GetRawElementPtr() + readCache.GetCount() - read));
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
		
		readCache.Clear();
		readCache.RequireCapacity(length + 1);

		int32 read=ReadBytes(length, readCache);
		ERR_ASSERT(read == length, u8"Error when read bytes!", return String::GetEmpty());

		return String((u8char*)readCache.GetRawElementPtr(), length);
	}
}
