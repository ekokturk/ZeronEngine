// Copyright (C) Eser Kokturk. All Rights Reserved.

#ifdef ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/11/PipelineD3D11.h>

#	include <d3d11.h>
#	include <Graphics/API/D3D/11/D3D11Helpers.h>
#	include <Graphics/API/D3D/11/GraphicsD3D11.h>
#	include <Graphics/API/D3D/11/ShaderD3D11.h>
#	include <Graphics/API/D3D/11/ShaderProgramD3D11.h>
#	include <Graphics/API/D3D/DebugInfoD3D.h>
#	include <Graphics/API/D3D/11/BufferD3D11.h>
#	include <Graphics/API/D3D/11/CommandBufferD3D11.h>

namespace Zeron::Gfx
{
	PipelineD3D11::PipelineD3D11(GraphicsD3D11& graphics, ShaderProgramD3D11* shader, RenderPass* renderPass, PipelineConfig&& pipelineConfig)
		: Pipeline(std::move(pipelineConfig))
		, mShader(shader)
		, mPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED)
	{
		auto* device = graphics.GetDeviceD3D();
		const PipelineConfig& config = *GetConfig();

		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FillMode = config.mSolidFill ? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;
		desc.CullMode = D3D11Helpers::GetCullMode(config.mCullMode);
		desc.FrontCounterClockwise = FALSE;
		ZE_D3D_ASSERT_RESULT(device->CreateRasterizerState(&desc, mRasterizerState.GetAddressOf()));

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		depthStencilDesc.DepthEnable = config.mDepthMode == DepthMode::Disabled ? false : true;
		depthStencilDesc.DepthWriteMask = config.mDepthMode == DepthMode::Default ? D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL :
																					D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		ZE_D3D_ASSERT_RESULT(device->CreateDepthStencilState(&depthStencilDesc, mDepthStencilState.GetAddressOf()));

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
		ZE_D3D_ASSERT_RESULT(device->CreateBlendState(&blendDesc, mBlendState.GetAddressOf()));

		switch (config.mTopology) {
			case PrimitiveTopology::TriangleList: {
				mPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			} break;
			case PrimitiveTopology::LineList: {
				mPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			} break;
			case PrimitiveTopology::PointList: {
				mPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			} break;
			default: {
				ZE_FAIL("Invalid D3D11 primitive topology cannot be set!");
			}
		}

		for (const auto& pushConstant : mShader->GetPushConstants()) {
			mPushConstantBuffers.emplace(
				pushConstant.mShaderStage, std::make_unique<BufferD3D11>(graphics, BufferType::Uniform, 1, pushConstant.mStride, nullptr, BufferUsageType::Dynamic)
			);
		}
	}

	PipelineD3D11::PipelineD3D11(GraphicsD3D11& graphics, ShaderProgramD3D11& shader)
		: Pipeline()
		, mShader(&shader)
	{
		ZE_ASSERT(mShader->GetShader(ShaderType::Compute), "PipelineD3D11: Expected compute shader for the pipeline");
	}

	void PipelineD3D11::Apply(ID3D11DeviceContext* deviceContext)
	{
		if (IsCompute()) {
			const ShaderD3D11* shaderDx = static_cast<ShaderD3D11*>(mShader->GetShader(ShaderType::Compute));
			ZE_D3D_ASSERT(deviceContext->CSSetShader(static_cast<ID3D11ComputeShader*>(shaderDx->GetShaderD3D()), nullptr, 0));
		}
		else {
			ZE_D3D_ASSERT(deviceContext->RSSetState(mRasterizerState.Get()));
			ZE_D3D_ASSERT(deviceContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0));
			ZE_D3D_ASSERT(deviceContext->OMSetBlendState(mBlendState.Get(), nullptr, UINT32_MAX));
			// ZE_D3D_ASSERT(deviceContext->OMSetBlendState(nullptr, nullptr, UINT32_MAX));
			ZE_D3D_ASSERT(deviceContext->IASetPrimitiveTopology(mPrimitiveTopology));
			ZE_ASSERT(mShader, "D3D11 pipeline shader is not available!");
			ZE_D3D_ASSERT(deviceContext->IASetInputLayout(mShader->GetInputLayoutD3D()));
			if (const ShaderD3D11* shaderDx = static_cast<ShaderD3D11*>(mShader->GetShader(ShaderType::Vertex))) {
				ZE_D3D_ASSERT(deviceContext->VSSetShader(static_cast<ID3D11VertexShader*>(shaderDx->GetShaderD3D()), nullptr, 0));
			}
			if (const ShaderD3D11* shaderDx = static_cast<ShaderD3D11*>(mShader->GetShader(ShaderType::Fragment))) {
				ZE_D3D_ASSERT(deviceContext->PSSetShader(static_cast<ID3D11PixelShader*>(shaderDx->GetShaderD3D()), nullptr, 0));
			}
		}
	}

	void PipelineD3D11::Clear(ID3D11DeviceContext* deviceContext)
	{
		if (IsCompute()) {
			if (mShader->GetShader(ShaderType::Compute)) {
				ZE_D3D_ASSERT(deviceContext->CSSetShader(nullptr, nullptr, 0));
			}
		}
		else {
			ZE_D3D_ASSERT(deviceContext->RSSetState(nullptr));
			ZE_D3D_ASSERT(deviceContext->OMSetDepthStencilState(nullptr, 0));
			ZE_D3D_ASSERT(deviceContext->OMSetBlendState(nullptr, nullptr, UINT32_MAX));
			ZE_D3D_ASSERT(deviceContext->IASetPrimitiveTopology(mPrimitiveTopology));
			ZE_D3D_ASSERT(deviceContext->IASetInputLayout(nullptr));

			if (mShader->GetShader(ShaderType::Vertex)) {
				ZE_D3D_ASSERT(deviceContext->VSSetShader(nullptr, nullptr, 0));
			}
			if (mShader->GetShader(ShaderType::Fragment)) {
				ZE_D3D_ASSERT(deviceContext->PSSetShader(nullptr, nullptr, 0));
			}

			// Clear existing push constants associated with the pipeline
			if (const PushConstant::Element* pushConstantInfo = mShader->GetPushConstant(ShaderType::Vertex)) {
				ID3D11Buffer* data[1] = { nullptr };
				ZE_D3D_ASSERT(deviceContext->VSSetConstantBuffers(pushConstantInfo->mTypedBinding, 1, data));
			}
			if (const PushConstant::Element* pushConstantInfo = mShader->GetPushConstant(ShaderType::Fragment)) {
				ID3D11Buffer* data[1] = { nullptr };
				ZE_D3D_ASSERT(deviceContext->VSSetConstantBuffers(pushConstantInfo->mTypedBinding, 1, data));
			}
		}
	}

	ShaderProgramD3D11* PipelineD3D11::GetShaderProgramD3D() const
	{
		return mShader;
	}

	void PipelineD3D11::SetPushConstantBuffer(ID3D11DeviceContext* deviceContext, const void* data, uint32_t stride, ShaderType shaderType)
	{
		const auto found = mPushConstantBuffers.find(shaderType);
		ZE_ASSERT(found != mPushConstantBuffers.end(), "PipelineD3D11: Expected Pipeline to have a push constant definition");
		const PushConstant::Element* pushConstantInfo = mShader->GetPushConstant(shaderType);
		BufferD3D11& buffer = *found->second;
		ZE_ASSERT(buffer.GetStride() >= stride, "PipelineD3D11: Push constant buffer size {} does not match for update data size {}!", buffer.GetStride(), stride);
		buffer.UpdateD3D(deviceContext, data, stride, 0, BufferUpdateRule::UnmapMemory);
		ID3D11Buffer* bufferD3D[1] = { buffer.GetBufferD3D() };
		switch (shaderType) {
			case ShaderType::Vertex: {
				ZE_D3D_ASSERT(deviceContext->VSSetConstantBuffers(pushConstantInfo->mTypedBinding, 1, bufferD3D));
			} break;
			case ShaderType::Fragment: {
				ZE_D3D_ASSERT(deviceContext->PSSetConstantBuffers(pushConstantInfo->mTypedBinding, 1, bufferD3D));
			} break;
			default: ZE_FAIL("PipelineD3D11: Push constant is not supported for shader type");
		}
	}


}
#endif