// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "GraphicsD3D11.h"

#include "Graphics/Primitives.h"
#include "Graphics/API/D3D/DebugInfoD3D.h"
#include "Graphics/API/D3D/11/GraphicsContextD3D11.h"
#include "Window/Window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

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

		InitializeShaders();
		SetRasterizerState();
		SetDepthStencilState();
		SetSamplerState();

		// TODO: Move this into texture class texture class
		int width, height, comp;
		if(stbi_uc* textData = stbi_load("Resources/Textures/test_texture.png", &width, &height, &comp, STBI_rgb_alpha))
		{
			D3D11_TEXTURE2D_DESC textDesc;
			textDesc.Width = width;
			textDesc.Height = height;
			textDesc.MipLevels = 1;
			textDesc.ArraySize = 1;
			textDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			textDesc.SampleDesc.Count = 1;
			textDesc.SampleDesc.Quality = 0;
			textDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
			textDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
			textDesc.CPUAccessFlags = 0;
			textDesc.MiscFlags = 0;
			
			D3D11_SUBRESOURCE_DATA bufferData;
			ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));
			bufferData.pSysMem = textData;
			bufferData.SysMemPitch = width * STBI_rgb_alpha;

			Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
			D3D_ASSERT_RESULT(mDevice->CreateTexture2D(&textDesc, &bufferData, texture.GetAddressOf()), false);
			D3D_ASSERT_RESULT(mDevice->CreateShaderResourceView(texture.Get(), nullptr, &mTexture), false);
			
			stbi_image_free(textData);
		}
		
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
#if ZE_GRAPHICS_D3D
		for(auto& ctx : mGraphicsContexes) {
			ctx->ClearBuffer(Color::Black);
			ctx->SetRenderTarget();
			Draw();
			ctx->SwapBuffers();
		}
#endif
	}

	void GraphicsD3D11::Draw()
	{
#if ZE_GRAPHICS_D3D
		D3D_ASSERT(mDeviceContext->IASetInputLayout(mShaders[0]->GetInputLayout()));
		D3D_ASSERT(mDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		D3D_ASSERT(mDeviceContext->VSSetShader(mShaders[0]->GetVertexShader(), nullptr, 0));
		D3D_ASSERT(mDeviceContext->PSSetShader(mShaders[0]->GetPixelShader(), nullptr, 0));

		mDeviceContext->PSSetShaderResources(0, 1, mTexture.GetAddressOf());
		
		for(auto& buffer : mVertexBuffers) {
			buffer->BindBuffer(*this);
			mDeviceContext->Draw(buffer->GetSize(), 0);
		}
#endif
	}

	void GraphicsD3D11::CreateVertexBuffer(const std::vector<Vertex>& data)
	{
#if ZE_GRAPHICS_D3D
		auto buffer = std::make_unique<VertexBufferD3D11>(*this, data);
		mVertexBuffers.emplace_back(std::move(buffer));
#endif
	}

	void GraphicsD3D11::CreateGraphicsContext(Window* window)
	{
#if ZE_GRAPHICS_D3D
		ZE_ASSERT(window, "Window is null, could not create graphics context!");
		mGraphicsContexes.emplace_back(std::make_unique<GraphicsContextD3D11>(window, *this));
#endif
	}

#if ZE_GRAPHICS_D3D
	void GraphicsD3D11::SetRasterizerState()
	{
		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

		// TODO: Take in parameters for culling and fill
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		//// Changes rasterizer vertex draw order from clockwise vertex positions
		//rasterizerDesc.FrontCounterClockwise = TRUE;
		D3D_ASSERT_RESULT(mDevice->CreateRasterizerState(&rasterizerDesc, mRasterizerState.GetAddressOf()));

		// TODO: Maybe seperate this
		D3D_ASSERT(mDeviceContext->RSSetState(mRasterizerState.Get()));
	}

	void GraphicsD3D11::SetDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		// TODO: Parameterize these
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL; // All = Stencil ON
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL; 
		D3D_ASSERT_RESULT(mDevice->CreateDepthStencilState(&depthStencilDesc, mDepthStencilState.GetAddressOf()));

		D3D_ASSERT(mDeviceContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0));
	}

	void GraphicsD3D11::SetSamplerState()
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP; // X Coord
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP; // Y Coord
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		D3D_ASSERT_RESULT(mDevice->CreateSamplerState(&samplerDesc, mSamplerState.GetAddressOf()));
		D3D_ASSERT(mDeviceContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf()));
	}

	bool GraphicsD3D11::InitializeShaders()
	{
		auto shader = std::make_shared<ShaderD3D11>();
		// TODO: Move semantic names to static values
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			//{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			{ "TEXTURE_COORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		// TODO: Add a static shader path
		// TODO: Fetch all shaders from resource path
		if (!shader->CreateVertexShader(*this, "Resources/Shaders/Default.vs.cso", layout, ARRAYSIZE(layout))) {
			return false;
		}
		if (!shader->CreatePixelShader(*this, "Resources/Shaders/Default.fs.cso")) {
			return false;
		}

		// TODO: Add shaders with ID
		mShaders.emplace_back(std::move(shader));
		return true;
	}

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
