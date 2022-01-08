// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once
#if ZE_GRAPHICS_D3D
#include "Graphics/API/D3D/DXGI.h"
#include "Graphics/Shader.h"

struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11VertexShader;
struct ID3D11InputLayout;
struct ID3D11PixelShader;
struct ID3D10Blob;

namespace Zeron
{
	class GraphicsD3D11;

	class ShaderD3D11 : public Shader {
	public:
		bool CreateVertexShader(GraphicsD3D11& graphics, std::string shaderName, D3D11_INPUT_ELEMENT_DESC* description, UINT elementNo);
		bool CreatePixelShader(GraphicsD3D11& graphics, std::string shaderName);

		ID3D11VertexShader* GetVertexShader() const;
		ID3D11PixelShader* GetPixelShader() const;
		ID3D11InputLayout* GetInputLayout() const;

	protected:
		ZE::ComPtr<ID3D11VertexShader> mVertexShader;
		ZE::ComPtr<ID3D11PixelShader> mPixelShader;
		
		ZE::ComPtr<ID3D10Blob> mShaderBuffer;
		ZE::ComPtr<ID3D11InputLayout> mInputLayout;
	};
}
#endif
