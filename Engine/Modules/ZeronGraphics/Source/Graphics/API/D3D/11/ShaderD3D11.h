// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D
#include <Graphics/API/D3D/DXGI.h>
#include <Graphics/Shader.h>
#include <Graphics/VertexLayout.h>

struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D10Blob;
struct ID3D11DeviceChild;
struct ID3D11InputLayout;
struct ID3D11PixelShader;
struct ID3D11VertexShader;

namespace Zeron
{
	class GraphicsD3D11;

	class ShaderD3D11 : public Shader {
	public:

		ShaderD3D11(GraphicsD3D11& graphics, ShaderType type, const ZE::ComPtr<ID3D10Blob>& buffer);

		ID3D11DeviceChild* GetShaderD3D() const;
		ID3D10Blob* GetShaderBufferD3D() const;

	protected:
		ZE::ComPtr<ID3D11DeviceChild> mShader;
		ZE::ComPtr<ID3D10Blob> mShaderBuffer;
	};

	class ShaderProgramD3D11 : public ShaderProgram {
	public:
		ShaderProgramD3D11(GraphicsD3D11& graphics, const std::string& shaderName, const std::string& shaderDirectory, const VertexLayout& layout);
		ShaderProgramD3D11(GraphicsD3D11& graphics, const std::string& shaderName, const std::shared_ptr<Shader>& vertexShader, 
			const std::shared_ptr<Shader>& fragmentShader, const VertexLayout& layout);

		ID3D11InputLayout* GetInputLayoutD3D() const;
	
	private:
		bool CreateInputLayout_(GraphicsD3D11& graphics, const VertexLayout& layout);
		std::vector<D3D11_INPUT_ELEMENT_DESC> GetVertexLayoutD3D(const VertexLayout& layout) const;
		DXGI_FORMAT GetVertexFormatD3D(VertexFormat format) const;
		
		ZE::ComPtr<ID3D11InputLayout> mInputLayout;

	};
}
#endif
