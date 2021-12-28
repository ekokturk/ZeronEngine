// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_D3D
#include "ShaderD3D11.h"
#include "GraphicsD3D11.h"
#include "Graphics/API/D3D/DebugInfoD3D.h"

#include "d3dcompiler.h"
#include <d3d11.h>

namespace Zeron
{
	bool ShaderD3D11::CreateVertexShader(GraphicsD3D11& graphics, std::string shaderName,
		D3D11_INPUT_ELEMENT_DESC* description, UINT elementNo)
	{
		const auto shaderPath = std::filesystem::path(RESOURCE_PATH + shaderName);
		if (std::filesystem::exists(shaderPath)) {
			const std::wstring widePath = shaderPath.wstring();
			D3D_ASSERT_RESULT(D3DReadFileToBlob(widePath.c_str(), mShaderBuffer.GetAddressOf()), false);
			D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateVertexShader(mShaderBuffer->GetBufferPointer(),
				mShaderBuffer->GetBufferSize(), nullptr, mVertexShader.GetAddressOf()), false);
			D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateInputLayout(description, elementNo, mShaderBuffer->GetBufferPointer(),
				mShaderBuffer->GetBufferSize(), mInputLayout.GetAddressOf()), false);
		}
		return false;
	}

	bool ShaderD3D11::CreatePixelShader(GraphicsD3D11& graphics, std::string shaderName)
	{
		const auto shaderPath = std::filesystem::path(RESOURCE_PATH + shaderName);
		if(std::filesystem::exists(shaderPath)) {
			D3D_ASSERT_RESULT(D3DReadFileToBlob(shaderPath.wstring().c_str(), mShaderBuffer.GetAddressOf()), false);
			D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreatePixelShader(mShaderBuffer->GetBufferPointer(),
				mShaderBuffer->GetBufferSize(), nullptr, mPixelShader.GetAddressOf()), false);
			return true;
		}
		return false;
	}

	ID3D11VertexShader* ShaderD3D11::GetVertexShader() const
	{
		return mVertexShader.Get();
	}

	ID3D11PixelShader* ShaderD3D11::GetPixelShader() const
	{
		return mPixelShader.Get();
	}

	ID3D11InputLayout* ShaderD3D11::GetInputLayout() const
	{
		return mInputLayout.Get();
	}
}
#endif
