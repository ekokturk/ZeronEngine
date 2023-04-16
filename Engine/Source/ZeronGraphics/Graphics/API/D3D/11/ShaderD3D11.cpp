// Copyright (C) Eser Kokturk. All Rights Reserved.


#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/11/ShaderD3D11.h>

#	include <d3d11.h>
#	include <d3dcommon.h>
#	include <d3dcompiler.h>
#	include <Graphics/API/D3D/11/GraphicsD3D11.h>
#	include <Graphics/API/D3D/DebugInfoD3D.h>
#	include <Graphics/VertexLayout.h>

namespace Zeron
{
	ShaderD3D11::ShaderD3D11(GraphicsD3D11& graphics, ShaderType type, const ZE::ComPtr<ID3D10Blob>& buffer)
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

	ShaderProgramD3D11::ShaderProgramD3D11(
		GraphicsD3D11& graphics, const std::string& shaderName, const VertexLayout& vertexLayout, const ResourceLayout& resourceLayout, const ByteBuffer& vertexShader,
		const ByteBuffer& fragmentShader, const ByteBuffer& computeShader
	)
		: ShaderProgram(shaderName, vertexLayout, resourceLayout)
	{
		ZE::ComPtr<ID3DBlob> buffer;
		if (!vertexShader.empty()) {
			ZE_D3D_ASSERT_RESULT(D3DCreateBlob(vertexShader.size(), buffer.GetAddressOf()));
			std::memcpy(buffer->GetBufferPointer(), vertexShader.data(), vertexShader.size());
			mVertexShader = std::make_unique<ShaderD3D11>(graphics, ShaderType::Vertex, buffer);
		}
		if (!fragmentShader.empty()) {
			ZE_D3D_ASSERT_RESULT(D3DCreateBlob(fragmentShader.size(), buffer.GetAddressOf()));
			std::memcpy(buffer->GetBufferPointer(), fragmentShader.data(), fragmentShader.size());
			mFragmentShader = std::make_unique<ShaderD3D11>(graphics, ShaderType::Fragment, buffer);
		}
		_createInputLayout(graphics);
	}

	ShaderProgramD3D11::ShaderProgramD3D11(
		GraphicsD3D11& graphics, const std::string& shaderName, const std::shared_ptr<Shader>& vertexShader, const std::shared_ptr<Shader>& fragmentShader,
		const VertexLayout& vertexLayout, const ResourceLayout& resourceLayout
	)
		: ShaderProgram(shaderName, vertexLayout, resourceLayout)
	{
		mVertexShader = std::static_pointer_cast<ShaderD3D11>(vertexShader);
		mFragmentShader = std::static_pointer_cast<ShaderD3D11>(fragmentShader);
		_createInputLayout(graphics);
	}

	ShaderProgramD3D11::~ShaderProgramD3D11() {}

	Shader* ShaderProgramD3D11::GetShader(ShaderType type) const
	{
		switch (type) {
			case ShaderType::Vertex: return mVertexShader.get();
			case ShaderType::Fragment: return mFragmentShader.get();
			default: ZE_FAIL("Shader type is not supported by D3D11");
		}
		return nullptr;
	}

	ID3D11InputLayout* ShaderProgramD3D11::GetInputLayoutD3D() const
	{
		return mInputLayout.Get();
	}

	bool ShaderProgramD3D11::_createInputLayout(GraphicsD3D11& graphics)
	{
		if (mVertexShader) {
			const std::vector<D3D11_INPUT_ELEMENT_DESC> desc = _getVertexLayoutD3D(mVertexLayout);
			ZE_D3D_ASSERT_RESULT(
				graphics.GetDeviceD3D()->CreateInputLayout(
					desc.data(),
					static_cast<UINT>(mVertexLayout.GetElements().size()),
					mVertexShader->GetShaderBufferD3D()->GetBufferPointer(),
					static_cast<UINT>(mVertexShader->GetShaderBufferD3D()->GetBufferSize()),
					mInputLayout.GetAddressOf()
				),
				false
			);
			return true;
		}
		return false;
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> ShaderProgramD3D11::_getVertexLayoutD3D(const VertexLayout& layout) const
	{
		int currentSlot = -1;
		uint32_t offset = 0;
		std::vector<D3D11_INPUT_ELEMENT_DESC> layoutD3D;
		for (const auto& e : layout.GetElements()) {
			if (currentSlot < e.mSlot) {
				currentSlot = e.mSlot;
				offset = 0;
			}
			D3D11_INPUT_ELEMENT_DESC desc;
			desc.Format = _getVertexFormatD3D(e.mFormat);
			desc.SemanticName = e.mName.c_str();
			desc.SemanticIndex = 0;
			desc.InputSlot = e.mSlot;
			desc.InputSlotClass = e.mIsInstanced ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = e.mIsInstanced ? 1 : 0;
			desc.AlignedByteOffset = offset;
			offset += VertexLayout::GetVertexFormatSize(e.mFormat);
			layoutD3D.emplace_back(desc);
		}
		return layoutD3D;
	}

	DXGI_FORMAT ShaderProgramD3D11::_getVertexFormatD3D(VertexFormat format) const
	{
		switch (format) {
			case VertexFormat::Float2: return DXGI_FORMAT_R32G32_FLOAT;
			case VertexFormat::Float3: return DXGI_FORMAT_R32G32B32_FLOAT;
			case VertexFormat::Color: return DXGI_FORMAT_R8G8B8A8_UNORM;
			case VertexFormat::Unknown:
			default: ZE_FAIL("D3D11 vertex input format is not supported!");
		}
		return DXGI_FORMAT_UNKNOWN;
	}
}
#endif