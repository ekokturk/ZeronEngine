// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "GraphicsD3D11.h"
#include "Graphics/API/D3D/DebugInfoD3D.h"
#include "Graphics/API/D3D/11/GraphicsContextD3D11.h"
#include "Window/Window.h"

namespace Zeron
{
	bool GraphicsD3D11::Init()
	{
#if ZE_GRAPHICS_D3D
		IDXGIAdapter* primaryAdapter = GetPrimaryAdapterDXGI();
		D3D_ASSERT_RESULT(primaryAdapter->GetParent(__uuidof(IDXGIFactory), &mFactory), false);
		
		UINT creationFlags = 0;
	#if defined(_DEBUG)
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



		return true;
#else
		return false;
#endif
	}

	bool GraphicsD3D11::Destroy()
	{
		return false;
	}

	void GraphicsD3D11::RenderFrame()
	{
		for(auto& ctx : mGraphicsContexes) {
			ctx->ClearBuffer(Color::Black);
			ctx->SwapBuffers();
		}
	}

	void GraphicsD3D11::CreateGraphicsContext(Window* window)
	{
		ZE_ASSERT(window, "Window is null, could not create graphics context!");
		mGraphicsContexes.emplace_back(std::make_unique<GraphicsContextD3D11>(window, *this));
	}

#if ZE_GRAPHICS_D3D
	ID3D11Device* GraphicsD3D11::GetDevice() const
	{
		return mDevice.Get();
	}

	ID3D11DeviceContext* GraphicsD3D11::GetDeviceContext() const
	{
		return mDeviceContext.Get();
	}
#endif
}
