#include "Engine/Application/Rendering/Renderer.h"
#include "Engine/Platform/Windows/BetterWindows.h"
#include "Engine/Platform/Windows/UnicodeHelper.h"
#include "Engine/Platform//Windows/Window.h"
#include "Engine/System/Memory/SharedPtr.h"
#include "Engine/System/Math/Color.h"
#include "Engine/Application/Engine.h"

namespace Engine {
	Renderer::Renderer() {
		driverTypeNames.Add(D3D_DRIVER_TYPE_HARDWARE, STRL("Hardware"));
		driverTypeNames.Add(D3D_DRIVER_TYPE_WARP, STRL("Warp"));
		driverTypeNames.Add(D3D_DRIVER_TYPE_REFERENCE, STRL("Reference"));

		featureLevelNames.Add(D3D_FEATURE_LEVEL_11_0, STRL("11.0"));
		featureLevelNames.Add(D3D_FEATURE_LEVEL_11_1, STRL("11.1"));

		const D3D_DRIVER_TYPE requiredDriverTypes[] = {
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};
		const sizeint requiredDriverTypeCount = sizeof(requiredDriverTypes) / sizeof(D3D_DRIVER_TYPE);

		const D3D_FEATURE_LEVEL requiredFeatureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0
		};
		const sizeint requiredFeatureLevelCount = sizeof(requiredFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

		D3D_FEATURE_LEVEL featureLevel;
		sizeint driverType;
		HRESULT hr;

		{
			const D3D11_CREATE_DEVICE_FLAG deviceFlag = D3D11_CREATE_DEVICE_DEBUG;

			ComPtr<ID3D11Device> d3dDevice0;
			ComPtr<ID3D11DeviceContext> d3dContext0;

			hr = E_FAIL;
			for (driverType = 0; driverType < requiredDriverTypeCount; driverType += 1) {
				hr = D3D11CreateDevice(
					nullptr, // Graphics card
					requiredDriverTypes[driverType],
					nullptr, // Software driver
					deviceFlag,
					requiredFeatureLevels,
					requiredFeatureLevelCount,
					D3D11_SDK_VERSION,
					d3dDevice0.GetAddressOf(),
					&featureLevel,
					d3dContext0.GetAddressOf()
				);
				if (SUCCEEDED(hr)) {
					break;
				}
			}
			FATAL_ASSERT(SUCCEEDED(hr), u8"Failed to create ID3D11Device");

			hr = d3dDevice0.As(&d3dDevice);
			FATAL_ASSERT(d3dDevice != nullptr, u8"Failed to get ID3D11Device1");
			hr = d3dContext0.As(&d3dContext);
			FATAL_ASSERT(d3dContext != nullptr, u8"Failed to get ID3D11DeviceContext1");

			hr = d3dDevice0.As(&dxgiDevice);
			FATAL_ASSERT(SUCCEEDED(hr), u8"Failed to get IDXGIDevice2");

			ComPtr<IDXGIAdapter> dxgiAdapter0;
			hr = dxgiDevice->GetAdapter(dxgiAdapter0.GetAddressOf());
			FATAL_ASSERT(dxgiAdapter0 != nullptr, u8"Failed to get IDXGIAdapter");
			hr = dxgiAdapter0.As(&dxgiAdapter);
			FATAL_ASSERT(dxgiAdapter != nullptr, u8"Failed to get IDXGIAdapter2");

			hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)dxgiFactory.GetAddressOf());
			FATAL_ASSERT(SUCCEEDED(hr), u8"Failed to get IDXGIFactory2");

			valid = true;
		}

		{
			DXGI_ADAPTER_DESC2 adapterDesc;
			dxgiAdapter->GetDesc2(&adapterDesc);
			String description;
			PlatformSpecific::UnicodeHelper::UnicodeToUTF8(adapterDesc.Description, description);
			INFO_MSG(String::Format(
				STRL("Renderer: Direct3D 11\nDriver: {0} - {1}\nFeature Level: {2}"),
				driverTypeNames.Get(requiredDriverTypes[driverType]),
				description,
				featureLevelNames.Get(featureLevel)
			).GetRawArray());
		}

		// Vertex Shader
		{
			ComPtr<ID3DBlob> shaderBlob;
			hr = CreateShaderFromFile(
				STRL("./Shaders/Triangle_VS.cso"),
				STRL("./Shaders/Triangle_VS.hlsl"),
				"VS", "vs_5_0", shaderBlob
			);
			FATAL_ASSERT(SUCCEEDED(hr), u8"Failed to load vertex shader from file!");

			hr = d3dDevice->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, d3dVertexShader.GetAddressOf());
			FATAL_ASSERT(SUCCEEDED(hr), u8"Failed to create vertex shader!");

			d3dDevice->CreateInputLayout(VertexPosColor::InputLayout, 2, shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), d3dVertexInputLayout.GetAddressOf());
			FATAL_ASSERT(SUCCEEDED(hr), u8"Failed to create vertex input layout!");

			d3dContext->IASetInputLayout(d3dVertexInputLayout.Get());
		}

		// Pixel Shader
		{
			ComPtr<ID3DBlob> shaderBlob;
			hr = CreateShaderFromFile(
				STRL("./Shaders/Triangle_PS.cso"),
				STRL("./Shaders/Triangle_PS.hlsl"),
				"PS", "ps_5_0", shaderBlob
			);
			FATAL_ASSERT(SUCCEEDED(hr), u8"Failed to load pixel shader from file!");

			hr = d3dDevice->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, d3dPixelShader.GetAddressOf());
			FATAL_ASSERT(SUCCEEDED(hr), u8"Failed to create pixel shader!");
		}

		// Vertex buffer
		{
			D3D11_BUFFER_DESC desc{};
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.ByteWidth = sizeof(vertices);
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA init{};
			init.pSysMem = vertices;

			hr = d3dDevice->CreateBuffer(&desc, &init, d3dVertexBuffer.GetAddressOf());
			FATAL_ASSERT(SUCCEEDED(hr), u8"Failed to create vertex buffer!");

			UINT stride = sizeof(VertexPosColor);
			UINT offset = 0;
			d3dContext->IASetVertexBuffers(0, 1, d3dVertexBuffer.GetAddressOf(), &stride, &offset);
		}
		
		// Index buffer
		{
			D3D11_BUFFER_DESC desc{};
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.ByteWidth = sizeof(indices);
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			desc.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA init{};
			init.pSysMem = indices;

			hr = d3dDevice->CreateBuffer(&desc, &init, d3dIndexBuffer.GetAddressOf());
			FATAL_ASSERT(SUCCEEDED(hr), u8"Failed to create index buffer!");

			d3dContext->IASetIndexBuffer(d3dIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		}

		// Transform matrices buffer
		{
			D3D11_BUFFER_DESC desc{};
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = sizeof(TransformMatrices);
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			hr = d3dDevice->CreateBuffer(&desc, nullptr, d3dTransformMatricesBuffer.GetAddressOf());
			FATAL_ASSERT(SUCCEEDED(hr), u8"Failed to create transform matrices buffer!");
		}

		d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	bool Renderer::IsValid() const {
		return valid;
	}
	bool Renderer::RegisterWindow(Window* window) {
		auto win = (PlatformSpecific::Window*)window;
		auto data = SharedPtr<WindowData>::Create();

#pragma region Connect OnResized signal with extra WindowId data.
		Variant argWindowId = win->GetId();
		const Variant* extraArgs[] = { &argWindowId };
		win->ConnectSignal(STRL("OnResized"), Invokable(this, STRL("DelegateOnWindowResized")), extraArgs, 1);
#pragma endregion

#pragma region SwapChainDesc
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
		swapChainDesc.Width = (UINT)64;
		swapChainDesc.Height = (UINT)64;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;
#pragma endregion

#pragma region FullscreenDesc
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc{};
		fullscreenDesc.RefreshRate.Numerator = 60;
		fullscreenDesc.RefreshRate.Denominator = 1;
		fullscreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		fullscreenDesc.Windowed = true;
#pragma endregion

		HRESULT hr;
		hr = dxgiFactory->CreateSwapChainForHwnd(d3dDevice.Get(), win->GetHWnd(), &swapChainDesc, &fullscreenDesc, nullptr, data->dxgiSwapChain.GetAddressOf());
		ERR_ASSERT(SUCCEEDED(hr), u8"Failed to create IDXGISwapChain1", return false);

		hr=dxgiFactory->MakeWindowAssociation(win->GetHWnd(), DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);
		ERR_ASSERT(SUCCEEDED(hr), u8"Failed to MakeWindowAssociation", return false);

		windowData.Add(win->GetId(), data);
		return true;
	}
	void Renderer::DelegateOnWindowResized(Vector2 size, WindowID windowId) {
		SharedPtr<WindowData> data;
		if (!windowData.TryGet(windowId, data)) {
			ERR_MSG(u8"Failed to get WindowData when recreating SwapChain.");
			return;
		}

		data->d3dRenderTargetView.Reset();
		data->d3dDepthStencilView.Reset();
		data->d3dDepthStencilTexture.Reset();

		HRESULT hr;
		hr = data->dxgiSwapChain->ResizeBuffers(1, (UINT)size.x, (UINT)size.y, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		ERR_ASSERT(SUCCEEDED(hr), u8"Failed to reset SwapChain buffers", return);
		ComPtr<ID3D11Texture2D> renderTargetTexture;
		hr = data->dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)renderTargetTexture.GetAddressOf());
		ERR_ASSERT(SUCCEEDED(hr), u8"Failed to get RenderTargetTexture", return);
		hr = d3dDevice->CreateRenderTargetView(renderTargetTexture.Get(), nullptr, data->d3dRenderTargetView.GetAddressOf());
		ERR_ASSERT(SUCCEEDED(hr), u8"Failed to create ID3D11RenderTargetView", return);

		D3D11_TEXTURE2D_DESC depthStencilTextureDesc{};
		depthStencilTextureDesc.Width = (int32)size.x;
		depthStencilTextureDesc.Height = (int32)size.y;
		depthStencilTextureDesc.MipLevels = 1;
		depthStencilTextureDesc.ArraySize = 1;
		depthStencilTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilTextureDesc.SampleDesc.Count = 1;
		depthStencilTextureDesc.SampleDesc.Quality = 0;
		depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilTextureDesc.CPUAccessFlags = 0;
		depthStencilTextureDesc.MiscFlags = 0;

		hr = d3dDevice->CreateTexture2D(&depthStencilTextureDesc, nullptr, data->d3dDepthStencilTexture.GetAddressOf());
		ERR_ASSERT(SUCCEEDED(hr), u8"Failed to create ID3D11Texutre2D for DepthStencilView", return);

		hr = d3dDevice->CreateDepthStencilView(data->d3dDepthStencilTexture.Get(), nullptr, data->d3dDepthStencilView.GetAddressOf());
		ERR_ASSERT(SUCCEEDED(hr), u8"Failed to create ID3D11DepthStencilView", return);

		data->size = size;
		//INFO_MSG(String::Format(STRL("Updated window size to: {0}"), ObjectUtil::ToString(size)).GetRawArray());
	}
	bool Renderer::UnregisterWindow(WindowID window) {
		SharedPtr<WindowData> data;
		if (windowData.TryGet(window, data)) {
			d3dContext->ClearState();
		}
		return windowData.Remove(window);
	}

	const D3D11_INPUT_ELEMENT_DESC Renderer::VertexPosColor::InputLayout[2] = {
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,3 * 4,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	HRESULT Renderer::CreateShaderFromFile(const String& csoFileName, const String& hlslFileName, const char* entryPoint, const char* shaderModel, ComPtr<ID3DBlob>& result) {
		bool succeeded;
		UniquePtr<WCHAR[]> csoFileNameW;
		succeeded = PlatformSpecific::UnicodeHelper::UTF8ToUnicode(csoFileName, csoFileNameW);
		ERR_ASSERT(succeeded, u8"Failed to convert String to WideString", return E_FAIL);
		UniquePtr<WCHAR[]> hlslFileNameW;
		succeeded = PlatformSpecific::UnicodeHelper::UTF8ToUnicode(hlslFileName, hlslFileNameW);
		ERR_ASSERT(succeeded, u8"Failed to convert String to WideString", return E_FAIL);

		// Return cache if exists.
		if (SUCCEEDED(D3DReadFileToBlob(csoFileNameW.GetRaw(), result.ReleaseAndGetAddressOf()))) {
			return S_OK;
		}

		// Compile shader
		DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
		shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
		
		ComPtr<ID3DBlob> errorBlob;
		HRESULT hr = D3DCompileFromFile(
			hlslFileNameW.GetRaw(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint,
			shaderModel,
			shaderFlags, 0, result.ReleaseAndGetAddressOf(), errorBlob.GetAddressOf()
		);
		if (SUCCEEDED(hr)) {
			// Write cache.
			hr=D3DWriteBlobToFile(result.Get(), csoFileNameW.GetRaw(), TRUE);
			if (FAILED(hr)) {
				ERR_MSG(u8"Failed to write HLSL shader cache!");
			}
			return S_OK;
		}

		const char* errorMsg = "No error message.";
		if (errorBlob != nullptr) {
			errorMsg = reinterpret_cast<const char*>(errorBlob->GetBufferPointer());
		}
		ERR_MSG(String::Format(STRL("Failed to compile HLSL shader({0}): {1}"), hr, errorMsg).GetRawArray());
		return hr;
	}
	
	void Renderer::Render() {
		double time = ENGINEINST->GetTime().GetTotal();

		for (const auto& data : windowData) {
			HRESULT hr;

			d3dContext->OMSetRenderTargets(1, data.value->d3dRenderTargetView.GetAddressOf(), data.value->d3dDepthStencilView.Get());
			
			{
				D3D11_VIEWPORT viewport{};
				viewport.TopLeftX = 0;
				viewport.TopLeftY = 0;
				viewport.Width = data.value->size.x;
				viewport.Height = data.value->size.y;
				viewport.MinDepth = 0;
				viewport.MaxDepth = 1;
				d3dContext->RSSetViewports(1, &viewport);
			}
			{
				Color clearColor = Color(0, 0, 0);
				d3dContext->ClearRenderTargetView(data.value->d3dRenderTargetView.Get(), (float*)&clearColor);
				d3dContext->ClearDepthStencilView(data.value->d3dDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			}
			{
				transform.world = DirectX::XMMatrixTranspose(
					DirectX::XMMatrixRotationX(time * 1) * DirectX::XMMatrixRotationY(time * 2)
				);
				transform.view = DirectX::XMMatrixTranspose(
					DirectX::XMMatrixLookAtLH(
						DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),//pos
						DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),//target
						DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)//up
					)
				);
				transform.projection = DirectX::XMMatrixTranspose(
					DirectX::XMMatrixPerspectiveFovLH(
						MathF::PI / 2, data.value->size.x / data.value->size.y, 1, 1000
					)
				);
				
				D3D11_MAPPED_SUBRESOURCE mapped;
				hr = d3dContext->Map(d3dTransformMatricesBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
				ERR_ASSERT(SUCCEEDED(hr), u8"Failed to map constant buffer!", continue);
				memcpy(mapped.pData, &transform, sizeof(TransformMatrices));
				d3dContext->Unmap(d3dTransformMatricesBuffer.Get(), 0);
			}
			{
				d3dContext->VSSetShader(d3dVertexShader.Get(), nullptr, 0);
				d3dContext->PSSetShader(d3dPixelShader.Get(), nullptr, 0);
				d3dContext->VSSetConstantBuffers(0, 1, d3dTransformMatricesBuffer.GetAddressOf());
			}

			d3dContext->DrawIndexed(36, 0,0);

			hr = data.value->dxgiSwapChain->Present(0, 0);
			ERR_ASSERT(SUCCEEDED(hr), u8"Failed to IDXGISwapChain::Present", continue);
		}
	}
}