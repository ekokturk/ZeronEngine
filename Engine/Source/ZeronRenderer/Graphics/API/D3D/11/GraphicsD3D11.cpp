// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "GraphicsD3D11.h"
#include "Graphics/Primitives.h"
#include "Graphics/API/D3D/DebugInfoD3D.h"
#include "Graphics/API/D3D/11/GraphicsContextD3D11.h"
#include "Window/Window.h"

#if ZE_GRAPHICS_D3D
#include <d3d11.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "SwapChainD3D11.h"
#include "TextureD3D11.h"
#include "stb/stb_image.h"

namespace Zeron
{
	GraphicsD3D11::GraphicsD3D11()
	{
	}

	GraphicsD3D11::~GraphicsD3D11()
	{
	}

	bool GraphicsD3D11::Init()
	{
#if ZE_GRAPHICS_D3D
		IDXGIAdapter* primaryAdapter = GetPrimaryAdapterD3D();
		D3D_ASSERT_RESULT(primaryAdapter->GetParent(__uuidof(IDXGIFactory), &mFactory), false);
		
		UINT creationFlags = 0;
	#if ZE_DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
		D3D_ASSERT_RESULT(D3D11CreateDevice(
			primaryAdapter,
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			creationFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&mDevice,
			nullptr,
			&mDeviceContext
		), false);

		ZE_LOGI("Graphics API: Direct3D 11");

		mImmediateContext = std::make_shared<GraphicsContextD3D11>(*this);

		//mConstantBuffer = std::make_unique<ConstantBufferD3D11<ConstantBufferVertex>>(*this, ConstantBufferVertex{ 0, 0.3f });
		return true;
#else
		return false;
#endif
	}

	std::shared_ptr<GraphicsContext> GraphicsD3D11::GetImmediateContext() const
	{
	#if ZE_GRAPHICS_D3D
		return mImmediateContext;
	#else
		return nullptr;
	#endif
	}

	GraphicsType GraphicsD3D11::GetGraphicsType() const
	{
		return GraphicsType::Direct3D11;
	}

	std::shared_ptr<GraphicsContext> GraphicsD3D11::CreateGraphicsContext()
	{
#if ZE_GRAPHICS_D3D
		return std::make_unique<GraphicsContextD3D11>(*this);
#else
		return nullptr;
#endif
	}

	std::shared_ptr<SwapChain> GraphicsD3D11::CreateSwapChain(Window& window)
	{
	#if ZE_GRAPHICS_D3D
		return std::make_shared<SwapChainD3D11>(window, *this);
	#else
		return nullptr;
	#endif
	}

	std::shared_ptr<Shader> GraphicsD3D11::CreateShader(const std::string& name)
	{
	#if ZE_GRAPHICS_D3D
		std::shared_ptr<ShaderD3D11> shader = std::make_shared<ShaderD3D11>();
		// TODO: Abstract this
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			//{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			{ "TEXTURE_COORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		shader->CreateVertexShader(*this, name + ".vs.cso", layout, ARRAYSIZE(layout));
		shader->CreatePixelShader(*this, name + ".fs.cso");
		return std::static_pointer_cast<Shader>(shader);
	#else
		return nullptr;
	#endif
	}

	std::shared_ptr<Texture> GraphicsD3D11::CreateTexture(TextureType type, const Color& data)
	{
	#if ZE_GRAPHICS_D3D
		return std::make_shared<TextureD3D11>(*this, type, &data, 1, 1);
	#else
		return nullptr;
	#endif
	}

	std::shared_ptr<Texture> GraphicsD3D11::CreateTexture(TextureType type, const Color* data, uint32_t width, uint32_t height)
	{
	#if ZE_GRAPHICS_D3D
		return std::make_shared<TextureD3D11>(*this, type, data, width, height);
	#else
		return nullptr;
	#endif
	}

	std::shared_ptr<Texture> GraphicsD3D11::CreateTexture(TextureType type, const std::string& path)
	{
	#if ZE_GRAPHICS_D3D
		return std::make_shared<TextureD3D11>(*this, type, path);
	#else
		return nullptr;
	#endif
	}

	std::shared_ptr<Buffer> GraphicsD3D11::CreateBuffer(BufferType type, const void* data, uint32_t size, uint32_t stride)
	{
	#if ZE_GRAPHICS_D3D
			return std::make_shared<BufferD3D11>(*this, type, data, size, stride);
	#else
			return nullptr;
	#endif
	}

#if ZE_GRAPHICS_D3D
	const std::vector<GraphicsAdapterD3D11>& GraphicsD3D11::GetAdapters()
	{
		if (mGraphicsAdapters.empty()) {
			ZE::ComPtr<IDXGIFactory> dxgiFactory;
			D3D_ASSERT_RESULT(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgiFactory.GetAddressOf())), mGraphicsAdapters);
			ZE::ComPtr<IDXGIAdapter>  dxgiAdapter;
			UINT index = 0;
			while (SUCCEEDED(dxgiFactory->EnumAdapters(index, &dxgiAdapter))) {
				mGraphicsAdapters.emplace_back(GraphicsAdapterD3D11(dxgiAdapter));
				index++;
			}
		}
		return mGraphicsAdapters;
	}

	IDXGIFactory* GraphicsD3D11::GetFactoryD3D() const
	{
		return mFactory.Get();
	}

	IDXGIAdapter* GraphicsD3D11::GetPrimaryAdapterD3D()
	{
		auto& adapterList = GetAdapters();
		ZE_ASSERT(!adapterList.empty(), "No graphics adapters found!");
		return adapterList[0].GetAdapterD3D();
	}

	ID3D11Device* GraphicsD3D11::GetDeviceD3D() const
	{
		return mDevice.Get();
	}

	ID3D11DeviceContext* GraphicsD3D11::GetDeviceContextD3D() const
	{
		return mDeviceContext.Get();
	}
#endif
}
