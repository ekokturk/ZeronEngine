// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/DXGI.h>
#	include <Graphics/ShaderProgram.h>
#	include <Graphics/VertexLayout.h>

struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D10Blob;
struct ID3D11DeviceChild;
struct ID3D11InputLayout;
struct ID3D11PixelShader;
struct ID3D11VertexShader;

namespace Zeron::Gfx
{
	struct ShaderProgramConfig;
	class ShaderD3D11;
	class GraphicsD3D11;

	class ShaderProgramD3D11 : public ShaderProgram {
	  public:
		ShaderProgramD3D11(GraphicsD3D11& graphics, const ShaderProgramConfig& config, std::unordered_map<ShaderType, ByteBuffer> shaderData);
		~ShaderProgramD3D11() override;

		// ShaderProgram
		Shader* GetShader(ShaderType type) const override;

		// D3D11 API
		ID3D11InputLayout* GetInputLayoutD3D() const;

	  private:
		bool _createInputLayout(GraphicsD3D11& graphics);
		std::vector<D3D11_INPUT_ELEMENT_DESC> _getVertexLayoutD3D(const std::vector<VertexLayout::Element>& layout) const;
		DXGI_FORMAT _getVertexFormatD3D(VertexFormat format) const;

	  private:
		std::shared_ptr<ShaderD3D11> mVertexShader;
		std::shared_ptr<ShaderD3D11> mFragmentShader;
		std::shared_ptr<ShaderD3D11> mComputeShader;
		Gfx::ComPtr<ID3D11InputLayout> mInputLayout;
	};
}
#endif