#pragma once
#include "Engine/System/Object/Object.h"
#include "Engine/Platform/Windows/Direct3D11.h"
#include "Engine/Application/Window.h"

namespace Engine {
	class Window;

	class Renderer:public ManualObject {
		REFLECTION_CLASS(::Engine::Renderer, ::Engine::ManualObject) {
			REFLECTION_METHOD(STRL("DelegateOnWindowResized"),
				Renderer::DelegateOnWindowResized,
				ARGLIST(STRL("size"), STRL("windowId")),
				ARGLIST()
			);
		}

	public:
		Renderer();
		~Renderer() = default;
		bool IsValid() const;
		bool RegisterWindow(Window* window);
		bool UnregisterWindow(WindowID window);
		void Render();
		
		void DelegateOnWindowResized(Vector2 size, WindowID windowId);

		HRESULT CreateShaderFromFile(const String& csoFileName, const String& hlslFileName, const char* entryPoint, const char* shaderModel, ComPtr<ID3DBlob>& result);

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

		ComPtr<ID3D11VertexShader> d3dVertexShader;
		ComPtr<ID3D11PixelShader> d3dPixelShader;

		struct VertexPosColor {
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT4 color;
			static const D3D11_INPUT_ELEMENT_DESC InputLayout[2];
		};
		ComPtr<ID3D11InputLayout> d3dVertexInputLayout;
		VertexPosColor vertices[3] = {
			{DirectX::XMFLOAT3(0.0f,0.5f,0.5f),DirectX::XMFLOAT4(0.0f,1.0f,0.0f,1.0f)},
			{DirectX::XMFLOAT3(0.5f,-0.5f,0.5f),DirectX::XMFLOAT4(0.0f,0.0f,1.0f,1.0f)},
			{DirectX::XMFLOAT3(-0.5f,-0.5f,0.5f),DirectX::XMFLOAT4(1.0f,0.0f,0.0f,1.0f)}
		};
		ComPtr<ID3D11Buffer> d3dVertexBuffer;
	};
}