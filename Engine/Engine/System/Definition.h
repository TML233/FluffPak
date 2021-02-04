#ifndef HEADER_CORE_DEFINITIONS
#define HEADER_CORE_DEFINITIONS

#pragma once
#include <cstdint>

namespace Engine {
	// Represents a 1-bit value.
	using Bool = bool;
	// Represents a signed 8-bit integer.
	using SByte = std::int8_t;
	// Represents an unsigned 8-bit integer.
	using Byte = std::uint8_t;
	// Represents a signed 16-bit integer.
	using Int16 = std::int16_t;
	// Represents an unsigned 16-bit integer.
	using UInt16 = std::uint16_t;
	// Represents a signed 32-bit integer.
	using Int32 = std::int32_t;
	// Represents an unsigned 32-bit integer.
	using UInt32 = std::uint32_t;
	// Represents a signed 64-bit integer.
	using Int64 = std::int64_t;
	// Represents an unsigned 64-bit integer.
	using UInt64 = std::uint64_t;
	// Represents a single float.
	using Float = float;
	// Represents a double float.
	using Double = double;
	// Represents a 8-bit character.
	using Char8 = Byte;
	// Represents a 16-bit character
	using Char16 = UInt16;
	// Represents a 32-bit character
	using Char32 = UInt32;
	// String
	class String;
}

#endif