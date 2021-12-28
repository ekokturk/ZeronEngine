// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#ifdef ZE_GRAPHICS_D3D
#include "GraphicsContextD3D11.h"
#include "Graphics/API/D3D/DebugInfoD3D.h"
#include "GraphicsD3D11.h"
#include "RenderTargetD3D11.h"
#include "TextureD3D11.h"

namespace Zeron {
	
	GraphicsContextD3D11::GraphicsContextD3D11(GraphicsD3D11& graphics)
		: mDevice(graphics.GetDeviceD3D())
		, mPrimitiveTopology(PrimitiveTopology::Invalid)
		, mRenderTarget(nullptr)
	{
		// TODO: Deffered Context
		mDeviceContext = graphics.GetDeviceContextD3D();

		// TODO: Move these to an object
		CreateRasterizerState(D3D11_FILL_SOLID);
		CreateDepthStencilState();
		CreateSamplerState();
	}

	GraphicsContextD3D11::~GraphicsContextD3D11()
	{
	}

	void GraphicsContextD3D11::SetRenderTarget(RenderTarget* target)
	{
		mRenderTarget = static_cast<RenderTargetD3D11*>(target);
		ID3D11RenderTargetView* renderView[] = { mRenderTarget->GetRenderTargetD3D() };
		D3D_ASSERT(mDeviceContext->OMSetRenderTargets(1, renderView, mRenderTarget->GetDepthStencilD3D()));
	}

	void GraphicsContextD3D11::SetShader(Shader* shader)
	{
		const auto* shaderApi = static_cast<ShaderD3D11*>(shader);
		if(auto* vertexShader = shaderApi->GetVertexShader()) {
			D3D_ASSERT(mDeviceContext->VSSetShader(vertexShader, nullptr, 0));
		}
		if(auto* pixelShader = shaderApi->GetPixelShader()) {
			D3D_ASSERT(mDeviceContext->PSSetShader(pixelShader, nullptr, 0));
		}
		D3D_ASSERT(mDeviceContext->IASetInputLayout(shaderApi->GetInputLayout()));
	}

	void GraphicsContextD3D11::SetTexture(Texture* texture)
	{
		const auto* textureApi = static_cast<TextureD3D11*>(texture);
		ID3D11ShaderResourceView* view[] = { textureApi->GetResourceViewD3D() };
		// TODO: handle slots
		mDeviceContext->PSSetShaderResources(0, 1, view);
	}

	void GraphicsContextD3D11::UpdateBuffer(Buffer& buff, void* data, uint32_t size)
	{
		const auto& buffApi = static_cast<BufferD3D11&>(buff);
		ZE_ASSERT(buff.GetBufferType() == BufferType::Constant, "Only constant buffers can be updated for now!");
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D_ASSERT_RESULT(mDeviceContext->Map(buffApi.GetBufferD3D(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		CopyMemory(mappedResource.pData, data, size);
		D3D_ASSERT(mDeviceContext->Unmap(buffApi.GetBufferD3D(), 0));
	}

	void GraphicsContextD3D11::SetPrimitiveTopology(PrimitiveTopology topology)
	{
		mPrimitiveTopology = topology;
		D3D_PRIMITIVE_TOPOLOGY toplogyApi = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
		switch (mPrimitiveTopology) {
			case PrimitiveTopology::TriangleList: {
				toplogyApi = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			} break;
			case PrimitiveTopology::PointList: {
				toplogyApi = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			} break;
			default: {
				mPrimitiveTopology = PrimitiveTopology::Invalid;
				ZE_FAIL("Invalid D3D11 primitive topology cannot be set!");
			}
		}
		D3D_ASSERT(mDeviceContext->IASetPrimitiveTopology(toplogyApi));
	}

	void GraphicsContextD3D11::SetVertexBuffer(Buffer& vb)
	{
		ZE_ASSERT(vb.GetBufferType() == BufferType::Vertex, "Invalid buffer type!");
		const auto& vbAPI = static_cast<BufferD3D11&>(vb);
		ID3D11Buffer* arr[] = { vbAPI.GetBufferD3D() };
		uint32_t stride[] = { vbAPI.GetStride() };
		uint32_t offset[] = { 0 };
		D3D_ASSERT(mDeviceContext->IASetVertexBuffers(0, 1, arr, stride, offset));
	}

	void GraphicsContextD3D11::SetIndexBuffer(Buffer& ib)
	{
		ZE_ASSERT(ib.GetBufferType() == BufferType::Index, "Invalid buffer type!");
		const auto& ibAPI = static_cast<BufferD3D11&>(ib);
		D3D_ASSERT(mDeviceContext->IASetIndexBuffer(ibAPI.GetBufferD3D(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0));
	}

	void GraphicsContextD3D11::SetConstantBuffer(Buffer& cb)
	{
		ZE_ASSERT(cb.GetBufferType() == BufferType::Constant, "Invalid buffer type!");
		const auto& cbAPI = static_cast<BufferD3D11&>(cb);
		ID3D11Buffer* arr[] = { cbAPI.GetBufferD3D() };
		D3D_ASSERT(mDeviceContext->VSSetConstantBuffers(0, 1, arr));
	}

	void GraphicsContextD3D11::SetFillMode(bool isSolid)
	{
		D3D11_RASTERIZER_DESC desc;
		mRasterizerState->GetDesc(&desc);
		const D3D11_FILL_MODE newMode = isSolid ? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;
		if(desc.FillMode != newMode) {
			CreateRasterizerState(newMode);
		}
	}

	void GraphicsContextD3D11::DrawIndexed(uint32_t indexCount, uint32_t indexStart)
	{
		D3D_ASSERT(mDeviceContext->DrawIndexed(indexCount, indexStart, 0));
	}

	void GraphicsContextD3D11::Clear(Color color)
	{
		if(mRenderTarget) {
			const float c[]{ color.normR(), color.normG(), color.normB(), color.normA() };
			D3D_ASSERT(mDeviceContext->ClearRenderTargetView(mRenderTarget->GetRenderTargetD3D(), c));
			D3D_ASSERT(mDeviceContext->ClearDepthStencilView(mRenderTarget->GetDepthStencilD3D(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0));
		}
	}

	void GraphicsContextD3D11::CreateRasterizerState(D3D11_FILL_MODE fillMode)
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

		desc.FillMode = fillMode;
		desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		//rasterizerDesc.FrontCounterClockwise = TRUE; // Changes rasterizer vertex draw order from clockwise vertex positions
		D3D_ASSERT_RESULT(mDevice->CreateRasterizerState(&desc, mRasterizerState.GetAddressOf()));

		// TODO: Maybe seperate this
		D3D_ASSERT(mDeviceContext->RSSetState(mRasterizerState.Get()));
	}

	void GraphicsContextD3D11::CreateDepthStencilState()
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

	void GraphicsContextD3D11::CreateSamplerState()
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
}
#endif
