// Copyright (C) Eser Kokturk. All Rights Reserved.


#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/11/ShaderD3D11.h>

#	include <d3d11.h>
#	include <d3dcommon.h>
#	include <d3dcompiler.h>
#	include <Graphics/API/D3D/11/GraphicsD3D11.h>
#	include <Graphics/API/D3D/DebugInfoD3D.h>

namespace Zeron::Gfx
{
	ShaderD3D11::ShaderD3D11(GraphicsD3D11& graphics, ShaderType type, const Gfx::ComPtr<ID3D10Blob>& buffer)
		: Shader(type)
		, mShaderBuffer(buffer)
	{
		switch (mType) {
			case ShaderType::Vertex: {
				ZE_D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateVertexShader(
					mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, reinterpret_cast<ID3D11VertexShader**>(mShader.GetAddressOf())
				));
			} break;
			case ShaderType::Fragment: {
				ZE_D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreatePixelShader(
					mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, reinterpret_cast<ID3D11PixelShader**>(mShader.GetAddressOf())
				));
			} break;
			case ShaderType::Compute: {
				ZE_D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateComputeShader(
					mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, reinterpret_cast<ID3D11ComputeShader**>(mShader.GetAddressOf())
				));
			} break;
		}
	}

	ShaderD3D11::~ShaderD3D11() {}

	ID3D11DeviceChild* ShaderD3D11::GetShaderD3D() const
	{
		return mShader.Get();
	}

	ID3DBlob* ShaderD3D11::GetShaderBufferD3D() const
	{
		return mShaderBuffer.Get();
	}
}
#endif