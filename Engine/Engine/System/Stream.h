#pragma once
#include "Engine/System/Object/Object.h"
#include <bit>

namespace Engine {
	class Stream :public ReferencedObject {
		REFLECTION_CLASS(::Engine::Stream, ::Engine::ReferencedObject) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}

	public:
		enum class Endianness {
			Little,
			Big
		};
		static constexpr Endianness LocalEndianness = (std::endian::native == std::endian::little ? Endianness::Little : Endianness::Big);

		virtual ~Stream() = default;

		/// @brief Get current endianness.\n
		/// This only indicates that how this Stream deal with data.\n
		/// Does not check whether the original stream is small-endian or big-endian. (And there's no way.)\n
		/// Default endianness will always be small-endian,
		Endianness GetCurrentEndianness() const;
		/// @brief Set current endianness.\n
		/// This affects how this Stream deal with data.\n
		/// Default endianness will always be small-endian.
		void SetCurrentEndianness(Endianness endian);


		virtual void Close() = 0;
		virtual bool IsValid() const = 0;
		virtual bool CanRead() const = 0;
		virtual bool CanWrite() const = 0;
		virtual bool CanRandomAccess() const = 0;

		virtual ResultCode SetPosition(int64 position) = 0;
		virtual int64 GetPosition() const = 0;
		virtual int64 GetLength() const = 0;


		/// @brief Write some bytes, byte by byte.
		ResultCode WriteBytes(const byte* valuePtr, int32 length);
		/// @brief Write some bytes, do conversions of endianness.
		ResultCode WriteBytesEndian(const byte* valuePtr, int32 length);

		ResultCode WriteByte(byte value);
		ResultCode WriteBytes(const List<byte>& value, int32 startIndex=0, int32 length=-1);
		ResultCode WriteSByte(sbyte value);
		ResultCode WriteInt16(int16 value);
		ResultCode WriteUInt16(uint16 value);
		ResultCode WriteInt32(int32 value);
		ResultCode WriteUInt32(uint32 value);
		ResultCode WriteInt64(int64 value);
		ResultCode WriteUInt64(uint64 value);
		ResultCode WriteFloat(float value);
		ResultCode WriteDouble(double value);
		/// @brief Write a String using C# BinaryWriter/Reader method:\n
		/// Starting with a 7-bit encoded integer as the length information, followed by the actual string data encoded in UTF-8.
		ResultCode WriteString(const String& value);
		ResultCode WriteText(const String& value);
		ResultCode WriteTextLine(const String& value);
		ResultCode Write7BitEncodedInt(int32 value);


		ResultCode TryReadBytes(int32 length, int32& readCount, List<byte>& result);
		ResultCode TryReadBytesEndian(int32 length, int32& readCount, List<byte>& result);

		
		/// @brief Read a String using C# BinaryWriter/Reader method:\n
		/// Starting with a 7-bit encoded integer as the length information, followed by the actual string data encoded in UTF-8.
		

	protected:
		/// @brief Implement this. No need to do the safe check.
		virtual ResultCode WriteBytesUnchecked(const byte* valuePtr, int32 length) = 0;
		/// @brief Implement this. No need to do the safe check.
		virtual ResultCode TryReadBytesUnchecked(int32 length, int32& readCount, List<byte>& result) = 0;

	private:
		static void SwapBuffer(byte* ptr, int32 length);
		Endianness currentEndianness = Endianness::Little;
		List<byte> rwCache;
	};
}
