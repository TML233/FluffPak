#pragma once

#include <cstdint>
#include <cstddef>

#define STATIC_CLASS(name)					\
name() = delete;							\
~name() = delete;							\
name(const name&) = delete;					\
name& operator=(const name&) = delete;		\
name(name&&) = delete;						\
name& operator=(name&&) = delete

namespace Engine {
	/// @brief Represents a char used for texts.
	using u8char = char8_t;
	/// @brief Represents a signed 8-bit integer.
	using sbyte = std::int8_t;
	/// @brief Represents an unsigned 8-bit integer.
	using byte = std::uint8_t;
	/// @brief Represents a signed 16-bit integer.
	using int16 = std::int16_t;
	/// @brief Represents an unsigned 16-bit integer.
	using uint16 = std::uint16_t;
	/// @brief Represents a signed 32-bit integer.
	using int32 = std::int32_t;
	/// @brief Represents an unsigned 32-bit integer.
	using uint32 = std::uint32_t;
	/// @brief Represents a signed 64-bit integer.
	using int64 = std::int64_t;
	/// @brief Represents an unsigned 64-bit integer.
	using uint64 = std::uint64_t;
	/// @brief Represents a local machine sized integer which can hold the memory size.
	using sizeint = std::size_t;

	enum class ResultCode {
		OK,
		UnknownError,
		InvalidArgument,
		NotSupported,
		NoPermission,
		NotFound,
		AlreadyExists,
		InvalidStream,
		OutOfMemory,
	};

	/// @brief A helper struct for storing result code and result value together.\n
	/// Only used for small values. Big values still remains being returned via the parameter.
	template<typename T>
	struct ResultPair {
		ResultPair(ResultCode result, T value) :result(result), value(value) {}
		ResultCode result;
		T value;
	};
}