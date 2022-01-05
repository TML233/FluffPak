#pragma once
#include "Engine/System/Object/Object.h"
#include "Engine/Platform/Windows/Direct3D11.h"
#include "Engine/Application/Window.h"

namespace Engine {
	class Window;

	class Renderer:public ManualObject {
	public:
		Renderer();
		~Renderer() = default;
		bool IsValid() const;
		bool RegisterWindow(Window* window);
		bool UnregisterWindow(WindowID window);
		void Render();

	private:
		bool valid = false;
		ComPtr<ID3D11Device1> d3dDevice1;
		ComPtr<ID3D11DeviceContext1> d3dContext1;

		ComPtr<IDXGIDevice2> dxgiDevice2;
		ComPtr<IDXGIAdapter2> dxgiAdapter2;
		ComPtr<IDXGIFactory2> dxgiFactory2;

		struct WindowData{
			ComPtr<IDXGISwapChain1> dxgiSwapChain1;
			ComPtr<ID3D11RenderTargetView> d3dRenderTargetView;
			ComPtr<ID3D11Texture2D> d3dDepthStencilTexture;
			ComPtr<ID3D11DepthStencilView> d3dDepthStencilView;
		};
		Dictionary<int32, SharedPtr<WindowData>> windowData;

		Dictionary<D3D_FEATURE_LEVEL, String> featureLevelNames;
		Dictionary<D3D_DRIVER_TYPE, String> driverTypeNames;
	};
}