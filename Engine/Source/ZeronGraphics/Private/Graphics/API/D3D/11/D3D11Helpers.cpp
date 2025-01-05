// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/11/D3D11Helpers.h>

#	include <d3d11.h>
#	include <Graphics/API/D3D/DebugInfoD3D.h>

namespace Zeron::Gfx
{
	D3D11_BIND_FLAG D3D11Helpers::GetBufferType(BufferType bufferType)
	{
		switch (bufferType) {
			case BufferType::Vertex: return D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
			case BufferType::Index: return D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
			case BufferType::Uniform: return D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
			case BufferType::Storage: return D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
			case BufferType::Undefined: return static_cast<D3D11_BIND_FLAG>(0);
			default: ZE_FAIL("D3D11 buffer type is not supported!");
		}
		return static_cast<D3D11_BIND_FLAG>(0);
	}

	D3D11_CULL_MODE D3D11Helpers::GetCullMode(FaceCullMode cullMode)
	{
		switch (cullMode) {
			case FaceCullMode::None: return D3D11_CULL_MODE::D3D11_CULL_NONE;
			case FaceCullMode::Back: return D3D11_CULL_MODE::D3D11_CULL_BACK;
			case FaceCullMode::Front: return D3D11_CULL_MODE::D3D11_CULL_FRONT;
			case FaceCullMode::FrontAndBack:;
			default: ZE_FAIL("D3D11 cull mode is not supported");
		}
	}

	UINT D3D11Helpers::GetMultiSampleLevel(MSAALevel level)
	{
		switch (level) {
			case MSAALevel::Disabled: return 1;
			case MSAALevel::x2: return 2;
			case MSAALevel::x4: return 4;
			case MSAALevel::x8: return 8;
			default: ZE_FAIL("D3D11 multi-sample anti aliasing level is not supported!");
		}
		return 0;
	}
	DXGI_FORMAT D3D11Helpers::GetTextureFormat(TextureFormat textureFormat)
	{
		switch (textureFormat) {

			case TextureFormat::RGB_8U: return DXGI_FORMAT_B8G8R8X8_UNORM;
			case TextureFormat::RGBA_8U: return DXGI_FORMAT_R8G8B8A8_UNORM;
			case TextureFormat::BGRA_8U: return DXGI_FORMAT_B8G8R8A8_UNORM;
			case TextureFormat::Depth_16U: return DXGI_FORMAT_R16_TYPELESS;
			case TextureFormat::Depth_32F: return DXGI_FORMAT_R32_TYPELESS;
			case TextureFormat::DepthStencil_32U: return DXGI_FORMAT_R24G8_TYPELESS;
			default: ZE_FAIL("Texture format is not supported.");
		}
		return DXGI_FORMAT_UNKNOWN;
	}
}
#endif