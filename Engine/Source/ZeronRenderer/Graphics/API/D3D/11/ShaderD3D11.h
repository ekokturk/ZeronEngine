// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#if ZE_GRAPHICS_D3D
#include "Graphics/Graphics.h"
#include "Graphics/API/D3D/ShaderD3D.h"
#include <wrl.h>
#include <d3d11.h>

namespace Zeron {
	class GraphicsD3D11;

	class ShaderD3D11 : public ShaderD3D {
	public:
		bool CreateVertexShader(GraphicsD3D11& graphics, std::string shaderName, D3D11_INPUT_ELEMENT_DESC* description, UINT elementNo);
		bool CreatePixelShader(GraphicsD3D11& graphics, std::string shaderName);

		[[nodiscard]] ID3D11VertexShader* GetVertexShader() const;
		[[nodiscard]] ID3D11PixelShader* GetPixelShader() const;
		[[nodiscard]] ID3D11InputLayout* GetInputLayout() const;

	protected:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		
		Microsoft::WRL::ComPtr<ID3D10Blob> mShaderBuffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
	};
}
#endif
