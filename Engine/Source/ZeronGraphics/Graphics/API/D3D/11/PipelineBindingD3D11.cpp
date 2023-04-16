// Copyright (C) Eser Kokturk. All Rights Reserved.

#ifdef ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/11/PipelineBindingD3D11.h>

#	include <d3d11.h>
#	include <Graphics/API/D3D/11/BufferD3D11.h>
#	include <Graphics/API/D3D/11/GraphicsD3D11.h>
#	include <Graphics/API/D3D/11/PipelineD3D11.h>
#	include <Graphics/API/D3D/11/SamplerD3D11.h>
#	include <Graphics/API/D3D/11/ShaderD3D11.h>
#	include <Graphics/API/D3D/11/TextureD3D11.h>
#	include <Graphics/API/D3D/DebugInfoD3D.h>

namespace Zeron
{
	PipelineBindingD3D11::PipelineBindingD3D11(GraphicsD3D11& graphics, PipelineD3D11& pipeline, const std::vector<BindingHandle>& bindings)
	{
		ShaderProgramD3D11* shader = pipeline.GetShaderProgramD3D();
		ZE_ASSERT(shader, "D3D11 shader is not found for pipeline binding!");
		const auto& resources = shader->GetResourceLayout().GetResources();
		ZE_ASSERT(bindings.size() == resources.size(), "D3D11 pipeline bindings should match resource layout!");
		for (size_t i = 0; i < resources.size(); ++i) {
			const auto& resource = resources[i];
			auto& binding = bindings[i];
			switch (resource.mType) {
				case PipelineResourceType::UniformBuffer: {
					if (auto* handle = std::get_if<UniformBindingHandle>(&binding)) {
						ZE_ASSERT(handle->mBuffer->GetBufferType() == BufferType::Uniform, "Invalid D3D11 buffer binding type!");
						const auto* bufferDx = static_cast<BufferD3D11*>(handle->mBuffer);
						mConstantBuffers.push_back({ resource.mBinding, resource.mShaderStage, { bufferDx->GetBufferD3D() } });
					}
				} break;
				case PipelineResourceType::Sampler: {
					if (auto* handle = std::get_if<SamplerBindingHandle>(&binding)) {
						const auto* samplerDx = static_cast<SamplerD3D11*>(handle->mSampler);
						mSamplers.push_back({ resource.mBinding, resource.mShaderStage, {}, { samplerDx->GetSamplerStateD3D() } });
					}
				} break;
				case PipelineResourceType::Texture: {
					if (auto* handle = std::get_if<TextureBindingHandle>(&binding)) {
						const auto* textureDx = static_cast<TextureD3D11*>(handle->mTexture);
						mTextures.push_back({ resource.mBinding, resource.mShaderStage, {}, {}, { textureDx->GetResourceViewD3D() } });
					}
				} break;
				case PipelineResourceType::DynamicUniformBuffer:;
				default: ZE_FAIL("D3D11 pipeline bindings is not supported!");
			}
		}
	}

	void PipelineBindingD3D11::Apply(ID3D11DeviceContext* deviceContext)
	{
		for (const auto& buffer : mConstantBuffers) {
			switch (buffer.mShaderStage) {
				case ShaderType::Vertex: {
					ZE_D3D_ASSERT(deviceContext->VSSetConstantBuffers(buffer.mSlot, static_cast<UINT>(buffer.mBuffer.size()), buffer.mBuffer.data()));
				} break;
				case ShaderType::Fragment: {
					ZE_D3D_ASSERT(deviceContext->PSSetConstantBuffers(buffer.mSlot, static_cast<UINT>(buffer.mBuffer.size()), buffer.mBuffer.data()));
				} break;
			}
		}
		for (const auto& sampler : mSamplers) {
			switch (sampler.mShaderStage) {
				case ShaderType::Vertex: {
					ZE_D3D_ASSERT(deviceContext->VSSetSamplers(sampler.mSlot, static_cast<UINT>(sampler.mSampler.size()), sampler.mSampler.data()));
				} break;
				case ShaderType::Fragment: {
					ZE_D3D_ASSERT(deviceContext->PSSetSamplers(sampler.mSlot, static_cast<UINT>(sampler.mSampler.size()), sampler.mSampler.data()));
				} break;
			}
		}
		for (const auto& texture : mTextures) {
			switch (texture.mShaderStage) {
				case ShaderType::Vertex: {
					ZE_D3D_ASSERT(deviceContext->VSSetShaderResources(texture.mSlot, static_cast<UINT>(texture.mTexture.size()), texture.mTexture.data()));
				} break;
				case ShaderType::Fragment: {
					ZE_D3D_ASSERT(deviceContext->PSSetShaderResources(texture.mSlot, static_cast<UINT>(texture.mTexture.size()), texture.mTexture.data()));
				} break;
			}
		}
	}
}
#endif