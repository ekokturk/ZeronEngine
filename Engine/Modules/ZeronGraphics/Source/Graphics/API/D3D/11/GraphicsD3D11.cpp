// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/API/D3D/11/GraphicsD3D11.h>

#include <Graphics/API/D3D/11/GraphicsContextD3D11.h>
#include <Graphics/API/D3D/11/BufferD3D11.h>
#include <Graphics/API/D3D/11/ShaderD3D11.h>
#include <Graphics/API/D3D/11/SwapChainD3D11.h>
#include <Graphics/API/D3D/11/TextureD3D11.h>
#include <Graphics/API/D3D/DebugInfoD3D.h>
#include <Graphics/Primitives.h>

#if ZE_GRAPHICS_D3D
#include <d3d11.h>
#endif

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

	std::shared_ptr<SwapChain> GraphicsD3D11::CreateSwapChain(void* windowHandle, const Vec2i& size)
	{
	#if ZE_GRAPHICS_D3D
		return std::make_shared<SwapChainD3D11>(*this, windowHandle, size);
	#else
		return nullptr;
	#endif
	}

	std::shared_ptr<ShaderProgram> GraphicsD3D11::CreateShaderProgram(const std::string& shaderName,
		const std::string& shaderDirectory, const VertexLayout& layout)
	{
	#if ZE_GRAPHICS_D3D
		return std::make_shared<ShaderProgramD3D11>(*this, shaderName, shaderDirectory, layout);
	#else
		return nullptr;
	#endif
	}

	std::shared_ptr<ShaderProgram> GraphicsD3D11::CreateShaderProgram(const std::string& shaderName,
		const std::shared_ptr<Shader>& vertexShader, const std::shared_ptr<Shader>& fragmentShader,
	    const VertexLayout& layout)
	{
	#if ZE_GRAPHICS_D3D
		return std::make_shared<ShaderProgramD3D11>(*this, shaderName, vertexShader, fragmentShader, layout);
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
