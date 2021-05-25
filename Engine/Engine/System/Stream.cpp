#include "Engine/System/Stream.h"

namespace Engine {
	Stream::Endianness Stream::GetCurrentEndianness() const {
		return currentEndianness;
	}
	void Stream::SetCurrentEndianness(Stream::Endianness endianness) {
		currentEndianness = endianness;
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
	ResultCode Stream::WriteString(const String& value) {
		return WriteBytes((byte*)value.GetStartPtr(), value.GetCount());
	}
	ResultCode Stream::WriteStringLine(const String& value) {
		auto result = WriteString(value);
		WriteByte((byte)'\n');
		return result;
	}
}
