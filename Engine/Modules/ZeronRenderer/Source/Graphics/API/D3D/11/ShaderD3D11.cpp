// Copyright (C) Eser Kokturk. All Rights Reserved.

#include "Graphics/GraphicsContext.h"
#if ZE_GRAPHICS_D3D
#include "ShaderD3D11.h"
#include "GraphicsD3D11.h"
#include "Graphics/API/D3D/DebugInfoD3D.h"

#include "d3dcompiler.h"
#include <d3d11.h>
#include <d3dcommon.h>

namespace Zeron
{
	ShaderD3D11::ShaderD3D11(GraphicsD3D11& graphics, ShaderType type, const ZE::ComPtr<ID3D10Blob>& buffer)
		: mShaderBuffer(buffer)
	{
		mType = type;
		switch (mType) {
			case ShaderType::Vertex: {
				D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateVertexShader(mShaderBuffer->GetBufferPointer(),
					mShaderBuffer->GetBufferSize(), nullptr, reinterpret_cast<ID3D11VertexShader**>(mShader.GetAddressOf())));
			} break;
			case ShaderType::Fragment: {
				D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreatePixelShader(mShaderBuffer->GetBufferPointer(),
					mShaderBuffer->GetBufferSize(), nullptr, reinterpret_cast<ID3D11PixelShader**>(mShader.GetAddressOf())));
			} break;
		}
	}

	ID3D11DeviceChild* ShaderD3D11::GetShaderD3D() const
	{
		return mShader.Get();
	}

	ID3DBlob* ShaderD3D11::GetShaderBufferD3D() const
	{
		return mShaderBuffer.Get();
	}

	ShaderProgramD3D11::ShaderProgramD3D11(GraphicsD3D11& graphics, const std::string& shaderName, const std::string& shaderDirectory, const VertexLayout& layout)
	{
		mName = shaderName;
		ZE::ComPtr<ID3DBlob> buffer;
		const auto shaderPath = std::filesystem::path(shaderDirectory);
		if(const auto filePath = shaderPath / (shaderName + ".vs.cso"); std::filesystem::exists(filePath)) {
			D3D_ASSERT_RESULT(D3DReadFileToBlob(filePath.wstring().c_str(), buffer.GetAddressOf()));
			mVertexShader = std::make_shared<ShaderD3D11>(graphics, ShaderType::Vertex, buffer);
		}
		if(const auto filePath = shaderPath / (shaderName + ".fs.cso"); std::filesystem::exists(filePath)) {
			D3D_ASSERT_RESULT(D3DReadFileToBlob(filePath.wstring().c_str(), buffer.GetAddressOf()));
			mFragmentShader = std::make_shared<ShaderD3D11>(graphics, ShaderType::Fragment, buffer);
		}
		CreateInputLayout_(graphics, layout);
	}

	ShaderProgramD3D11::ShaderProgramD3D11(GraphicsD3D11& graphics, const std::string& shaderName, const std::shared_ptr<Shader>& vertexShader,
		const std::shared_ptr<Shader>& fragmentShader, const VertexLayout& layout)
	{
		mName = shaderName;
		mVertexShader = vertexShader;
		mFragmentShader = fragmentShader;
		CreateInputLayout_(graphics, layout);
	}

	ID3D11InputLayout* ShaderProgramD3D11::GetInputLayoutD3D() const
	{
		return mInputLayout.Get();
	}

	bool ShaderProgramD3D11::CreateInputLayout_(GraphicsD3D11& graphics, const VertexLayout& layout)
	{
		if (auto* vs = static_cast<ShaderD3D11*>(mVertexShader.get())) {
			const std::vector<D3D11_INPUT_ELEMENT_DESC> desc = GetVertexLayoutD3D(layout);
			D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateInputLayout(desc.data(),
				static_cast<UINT>(layout.GetElements().size()), vs->GetShaderBufferD3D()->GetBufferPointer(),
				static_cast<UINT>(vs->GetShaderBufferD3D()->GetBufferSize()), mInputLayout.GetAddressOf()), false);
			return true;
		}
		return false;
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> ShaderProgramD3D11::GetVertexLayoutD3D(const VertexLayout& layout) const
	{
		int currentSlot = -1;
		uint32_t offset = 0;
		std::vector<D3D11_INPUT_ELEMENT_DESC> layoutD3D;
		for (const auto& e : layout.GetElements()) {
			if(currentSlot < e.mSlot) {
				currentSlot = e.mSlot;
				offset = 0;
			}
			D3D11_INPUT_ELEMENT_DESC desc;
			desc.Format = GetVertexFormatD3D(e.mFormat);
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

	DXGI_FORMAT ShaderProgramD3D11::GetVertexFormatD3D(VertexFormat format) const
	{
		switch (format) {
		case VertexFormat::Float2: return DXGI_FORMAT_R32G32_FLOAT;
		case VertexFormat::Float3: return DXGI_FORMAT_R32G32B32_FLOAT;
		case VertexFormat::Unknown:
		default: return DXGI_FORMAT_UNKNOWN;
		}
	}
}
#endif
