#ifndef HEADER_CORE_DEFINITIONS
#define HEADER_CORE_DEFINITIONS

#pragma once
#include <cstdint>

#ifndef ENGINE_API
#define ENGINE_API
#endif

namespace Engine {
	// Represents a 1-bit value.
	using Bool = bool;
	// Represents a signed 8-bit integer.
	using SByte = std::int8_t;
	// Represents an unsigned 8-bit integer.
	using Byte = std::uint8_t;
	// Represents a signed 16-bit integer.
	using Short = std::int16_t;
	// Represents an unsigned 16-bit integer.
	using UShort = std::uint16_t;
	// Represents a signed 32-bit integer.
	using Int = std::int32_t;
	// Represents an unsigned 32-bit integer.
	using UInt = std::uint32_t;
	// Represents a signed 64-bit integer.
	using Long = std::int64_t;
	// Represents an unsigned 64-bit integer.
	using ULong = std::uint64_t;
	// Represents a single float.
	using Float = float;
	// Represents a double float.
	using Double = double;
	// Represents a 8-bit character.
	using Char = char;
	// String
	class String;
}

#endif