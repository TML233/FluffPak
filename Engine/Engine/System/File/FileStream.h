#pragma once
#include "Engine/System/File/FileSystem.h"

namespace Engine {
	class FileStream :public ReferencedObject {
		REFLECTION_CLASS(::Engine::FileStream, ::Engine::ReferencedObject) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}

	public:
		virtual ~FileStream() = default;

		/// @brief Get current endianness.\n
		/// This only indicates that how this File instance deal with data.\n
		/// Does not check whether the original file is small-endian or big-endian. (And there's no way.)
		/// Default endianness will always be small-endian,
		FileSystem::Endianness GetCurrentEndianness() const;
		/// @brief Set current endianness.\n
		/// This affects how this File instance deal with data.
		/// Default endianness will always be small-endian.
		void SetCurrentEndianness(FileSystem::Endianness endian);


		virtual void Close() = 0;
		virtual bool IsValid() const = 0;
		virtual bool CanRead() const = 0;
		virtual bool CanWrite() const = 0;

		virtual FileSystem::Result SetPosition(int64 position) = 0;
		virtual int64 GetPosition() const = 0;
		virtual int64 GetLength() const = 0;


		
		/// @brief Write some bytes, byte by byte.
		virtual FileSystem::Result WriteBytes(const byte* valuePtr, int64 length) = 0;

		virtual FileSystem::Result WriteByte(byte value) = 0;
		/// @brief Write a list of bytes, byte by byte.
		virtual FileSystem::Result WriteBytes(const List<byte>& value) = 0;
		virtual FileSystem::Result WriteSByte(sbyte value) = 0;
		virtual FileSystem::Result WriteInt16(int16 value) = 0;
		virtual FileSystem::Result WriteUInt16(uint16 value) = 0;
		virtual FileSystem::Result WriteInt32(int32 value) = 0;
		virtual FileSystem::Result WriteUInt32(uint32 value) = 0;
		virtual FileSystem::Result WriteInt64(int64 value) = 0;
		virtual FileSystem::Result WriteUInt64(uint64 value) = 0;
		virtual FileSystem::Result WriteFloat(float value) = 0;
		virtual FileSystem::Result WriteDouble(double value) = 0;
		virtual FileSystem::Result WriteString(const String& value) = 0;
		virtual FileSystem::Result WriteStringLine(const String& value) = 0;


		/// @brief Read some bytes, byte by byte.
		virtual int64 ReadBytes(int64 length, List<byte>& result) = 0;

		virtual byte ReadByte() = 0;
		virtual sbyte ReadSByte() = 0;
		virtual int16 ReadInt16() = 0;
		virtual uint16 ReadUInt16() = 0;
		virtual int32 ReadInt32() = 0;
		virtual uint32 ReadUInt32() = 0;
		virtual int64 ReadInt64() = 0;
		virtual uint64 ReadUInt64() = 0;
		virtual float ReadFloat() = 0;
		virtual double ReadDouble() = 0;
		virtual String ReadStringLine() = 0;
		virtual String ReadAllString() = 0;

	private:
		FileSystem::Endianness currentEndianness = FileSystem::Endianness::Little;
	};

}