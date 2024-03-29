#pragma once

#include "Engine/Platform/Windows/NoWindowsCrapStart.h"

#include <wrl.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

namespace Engine {
	template<typename T>
	using ComPtr = ::Microsoft::WRL::ComPtr<T>;
}

#include "Engine/Platform/Windows/NoWindowsCrapEnd.h"
