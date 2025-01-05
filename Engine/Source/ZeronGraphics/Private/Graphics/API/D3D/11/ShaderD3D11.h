// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/DXGI.h>
#	include <Graphics/Shader.h>

struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D10Blob;
struct ID3D11DeviceChild;
struct ID3D11InputLayout;
struct ID3D11PixelShader;
struct ID3D11VertexShader;

namespace Zeron::Gfx
{
	class GraphicsD3D11;

	class ShaderD3D11 : public Shader {
	  public:
		ShaderD3D11(GraphicsD3D11& graphics, ShaderType type, const Gfx::ComPtr<ID3D10Blob>& buffer);
		~ShaderD3D11();

		// D3D11 API
		ID3D11DeviceChild* GetShaderD3D() const;
		ID3D10Blob* GetShaderBufferD3D() const;

	  protected:
		Gfx::ComPtr<ID3D11DeviceChild> mShader;
		Gfx::ComPtr<ID3D10Blob> mShaderBuffer;
	};
}
#endif