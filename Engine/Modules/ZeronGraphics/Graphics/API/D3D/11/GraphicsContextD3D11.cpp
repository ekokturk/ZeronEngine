// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/API/D3D/11/GraphicsContextD3D11.h>

#ifdef ZE_GRAPHICS_D3D
#include <Graphics/API/D3D/11/BufferD3D11.h>
#include <Graphics/API/D3D/11/GraphicsD3D11.h>
#include <Graphics/API/D3D/11/RenderTargetD3D11.h>
#include <Graphics/API/D3D/11/ShaderD3D11.h>
#include <Graphics/API/D3D/11/SwapChainD3D11.h>
#include <Graphics/API/D3D/11/TextureD3D11.h>
#include <Graphics/API/D3D/DebugInfoD3D.h>

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
		CreateBlendState();
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

	void GraphicsContextD3D11::Draw(uint32_t vertexCount, uint32_t vertexStart)
	{
		D3D_ASSERT(mDeviceContext->Draw(vertexCount, vertexStart));
	}

	void GraphicsContextD3D11::SetShader(Shader* shader)
	{
		if(!shader) {
			return;
		}
		const auto* shaderD3D = static_cast<ShaderD3D11*>(shader);
		if(shaderD3D->GetType() == ShaderType::Vertex) {
			D3D_ASSERT(mDeviceContext->VSSetShader(static_cast<ID3D11VertexShader*>(shaderD3D->GetShaderD3D()), nullptr, 0));
		}
		else if(shaderD3D->GetType() == ShaderType::Fragment) {
			D3D_ASSERT(mDeviceContext->PSSetShader(static_cast<ID3D11PixelShader*>(shaderD3D->GetShaderD3D()), nullptr, 0));
		}
	}

	void GraphicsContextD3D11::SetShaderProgram(ShaderProgram* program)
	{
		if(!program) {
			return;
		}
		const auto* programD3D = static_cast<ShaderProgramD3D11*>(program);
		SetShader(programD3D->GetShader(ShaderType::Vertex));
		SetShader(programD3D->GetShader(ShaderType::Fragment));
		D3D_ASSERT(mDeviceContext->IASetInputLayout(programD3D->GetInputLayoutD3D()));
	}

	void GraphicsContextD3D11::SetTexture(Texture* texture, uint8_t slot)
	{
		if(!texture) {
			// TODO: Set null texture here maybe? Pink material
			return;
		}
		const auto* textureApi = static_cast<TextureD3D11*>(texture);
		ID3D11ShaderResourceView* view[] = { textureApi->GetResourceViewD3D() };
		mDeviceContext->PSSetShaderResources(slot, 1, view);
	}

	void GraphicsContextD3D11::UpdateBuffer(Buffer& buff, void* data, uint32_t sizeBytes)
	{
		const auto& buffApi = static_cast<BufferD3D11&>(buff);
		ZE_ASSERT(buff.GetBufferType() == BufferType::Constant, "Only constant buffers can be updated for now!");
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D_ASSERT_RESULT(mDeviceContext->Map(buffApi.GetBufferD3D(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		CopyMemory(mappedResource.pData, data, sizeBytes);
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

	void GraphicsContextD3D11::SetVertexBuffer(Buffer& vb, uint32_t slot)
	{
		ZE_ASSERT(vb.GetBufferType() == BufferType::Vertex, "Invalid buffer type!");
		const auto& vbAPI = static_cast<BufferD3D11&>(vb);
		ID3D11Buffer* arr[] = { vbAPI.GetBufferD3D() };
		uint32_t stride[] = { vbAPI.GetStride() };
		uint32_t offset[] = { 0 };
		D3D_ASSERT(mDeviceContext->IASetVertexBuffers(slot, 1, arr, stride, offset));
	}

	void GraphicsContextD3D11::SetVertexBuffers(Buffer** vb, uint32_t count, uint32_t slot)
	{
		//ZE_ASSERT(vb.GetBufferType() == BufferType::Vertex, "Invalid buffer type!");
		std::vector<ID3D11Buffer*> arr;
		std::vector<uint32_t> stride;
		std::vector<uint32_t> offset;
		for(uint32_t i = 0; i < count; ++i) {
			const BufferD3D11* vbAPI = static_cast<BufferD3D11*>(vb[i]);
			arr.push_back(vbAPI->GetBufferD3D());
			stride.push_back(vbAPI->GetStride());
			offset.push_back(0);
		}
		D3D_ASSERT(mDeviceContext->IASetVertexBuffers(slot, count, arr.data(), stride.data(), offset.data()));
	}

	void GraphicsContextD3D11::SetIndexBuffer(Buffer& ib)
	{
		ZE_ASSERT(ib.GetBufferType() == BufferType::Index, "Invalid buffer type!");
		const auto& ibAPI = static_cast<BufferD3D11&>(ib);
		D3D_ASSERT(mDeviceContext->IASetIndexBuffer(ibAPI.GetBufferD3D(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0));
	}

	void GraphicsContextD3D11::SetConstantBuffer(Buffer& cb, ShaderType type, uint32_t slot)
	{
		ZE_ASSERT(cb.GetBufferType() == BufferType::Constant, "Invalid buffer type!");
		const auto& cbAPI = static_cast<BufferD3D11&>(cb);
		ID3D11Buffer* arr[] = { cbAPI.GetBufferD3D() };
		switch (type) {
			case ShaderType::Vertex: {
				D3D_ASSERT(mDeviceContext->VSSetConstantBuffers(slot, 1, arr));
			} break;
			case ShaderType::Fragment: {
				D3D_ASSERT(mDeviceContext->PSSetConstantBuffers(slot, 1, arr));
			} break;
			default: ZE_FAIL("Shader type is not supported for Constant buffer!");
		}
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

	void GraphicsContextD3D11::DrawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexStart,
		uint32_t instanceStart)
	{
		D3D_ASSERT(mDeviceContext->DrawInstanced(vertexCount, instanceCount, vertexStart, instanceStart));
	}

	void GraphicsContextD3D11::DrawInstancedIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexStart,
		uint32_t baseVertexLocation, uint32_t instanceStart)
	{
		D3D_ASSERT(mDeviceContext->DrawIndexedInstanced(indexCount, instanceCount, indexStart, baseVertexLocation, instanceStart));
	}

	void GraphicsContextD3D11::Clear(Color color)
	{
		if(mRenderTarget) {
			const float c[]{ color.normR(), color.normG(), color.normB(), color.normA() };
			D3D_ASSERT(mDeviceContext->ClearRenderTargetView(mRenderTarget->GetRenderTargetD3D(), c));
			D3D_ASSERT(mDeviceContext->ClearDepthStencilView(mRenderTarget->GetDepthStencilD3D(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0));
		}
	}

	void GraphicsContextD3D11::SetViewport(const Vec2i& size, const Vec2i& position)
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = static_cast<FLOAT>(position.X);
		viewport.TopLeftY = static_cast<FLOAT>(position.Y);
		viewport.Width = static_cast<FLOAT>(size.X);
		viewport.Height = static_cast<FLOAT>(size.Y);
		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;

		// TODO: Multiple viewports
		D3D_ASSERT(mDeviceContext->RSSetViewports(1, &viewport));
	}

	void GraphicsContextD3D11::ResizeSwapChain(SwapChain& swapChain, const Vec2i& size)
	{
		auto& swapChainAPI = static_cast<SwapChainD3D11&>(swapChain);
		if(auto* targetAPI = static_cast<RenderTargetD3D11*>(swapChainAPI.GetRenderTarget())) {
			targetAPI->ReleaseBuffers();
			swapChainAPI.Resize(size);
			D3D_ASSERT(targetAPI->CreateBuffers(mDevice, swapChainAPI));
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

	void GraphicsContextD3D11::CreateBlendState()
	{
		D3D11_RENDER_TARGET_BLEND_DESC rtbDesc;
		ZeroMemory(&rtbDesc, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
		
		rtbDesc.BlendEnable = true;
		rtbDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

		// Source Pixel * Source Blend (OP) Destination Pixel * Destination Blend
		rtbDesc.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		rtbDesc.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		rtbDesc.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		
		// Source Alpha * Source Blend (OP) Destination Alpha * Destination Blend
		rtbDesc.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		rtbDesc.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		rtbDesc.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		

		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
		blendDesc.RenderTarget[0] = rtbDesc;
		
		D3D_ASSERT_RESULT(mDevice->CreateBlendState(&blendDesc, mBlendState.GetAddressOf()));
		//D3D_ASSERT(mDeviceContext->OMSetBlendState(mBlendState.Get(), nullptr, UINT32_MAX));
		D3D_ASSERT(mDeviceContext->OMSetBlendState(nullptr, nullptr, UINT32_MAX));

	}
}
#endif
