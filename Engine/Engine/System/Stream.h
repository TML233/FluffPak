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
		/// This only indicates that how this File instance deal with data.\n
		/// Does not check whether the original file is small-endian or big-endian. (And there's no way.)
		/// Default endianness will always be small-endian,
		Endianness GetCurrentEndianness() const;
		/// @brief Set current endianness.\n
		/// This affects how this File instance deal with data.
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
		/// @brief Read some bytes, byte by byte.
		/// @return number of bytes read.
		int32 ReadBytes(int32 length, List<byte>& result);
		/// @brief Read some bytes, do conversions of endianness.
		/// @return number of bytes read.
		int32 ReadBytesEndian(int32 length, List<byte>& result);


		ResultCode WriteByte(byte value);
		ResultCode WriteBytes(const List<byte>& value);
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
		/// Starting with a 7-bit encoded integer as the length information,\n
		/// followed by the actual string data encoded in UTF-8.
		//ResultCode WriteString(const String& value);

		ResultCode WriteText(const String& value);
		ResultCode WriteTextLine(const String& value);

		byte ReadByte();
		sbyte ReadSByte();
		int16 ReadInt16();
		uint16 ReadUInt16();
		int32 ReadInt32();
		uint32 ReadUInt32();
		int64 ReadInt64();
		uint64 ReadUInt64();
		float ReadFloat();
		double ReadDouble();
		/// @brief Read a String using C# BinaryWriter/Reader method:\n
		/// Starting with a 7-bit encoded integer as the length information,\n
		/// followed by the actual string data encoded in UTF-8.
		//String ReadString();

		String ReadTextLine();
		String ReadAllText();

	protected:
		/// @brief Implement this. No need to do the safe check.
		virtual ResultCode WriteBytesUnchecked(const byte* valuePtr, int32 length) = 0;
		/// @brief Implement this. No need to do the safe check.
		virtual int32 ReadBytesUnchecked(int32 length, List<byte>& result) = 0;

	private:
		Endianness currentEndianness = Endianness::Little;
		List<byte> readCache;
	};
}
