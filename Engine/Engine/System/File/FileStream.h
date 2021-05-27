#pragma once
#include "Engine/System/Stream.h"

namespace Engine {
	template<typename T>
	class List;

	class FileStream :public Stream {
		REFLECTION_CLASS(::Engine::FileStream, ::Engine::Stream) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}

	public:
		enum class OpenMode :byte {
			/// @brief Read-only.
			ReadOnly,
			/// @brief Write-only, clears the file.
			WriteTruncate,
			/// @brief Write-only, append to the file.
			WriteAppend,
			/// @brief Read-write, clears the file.
			ReadWriteTruncate,
			/// @brief Read-write, append to the file.
			ReadWriteAppend
		};

		static bool IsOpenModeValid(OpenMode mode);
		static bool IsOpenModeReadOnly(OpenMode mode);
		static bool IsOpenModeRead(OpenMode mode);
		static bool IsOpenModeWrite(OpenMode mode);
		static bool IsOpenModeTruncate(OpenMode mode);
		static bool IsOpenModeAppend(OpenMode mode);

		bool CanRandomAccess() const override;
	};

}