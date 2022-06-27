// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/API/D3D/11/BufferD3D11.h>

#if ZE_GRAPHICS_D3D
#include <d3d11.h>
#include <Graphics/API/D3D/DebugInfoD3D.h>
#include <Graphics/API/D3D/11/GraphicsD3D11.h>

namespace Zeron
{
	BufferD3D11::BufferD3D11(GraphicsD3D11& graphics, BufferType type, const void* data, uint32_t count, uint32_t stride)
		: mBufferType(type)
	{
		mCount = count;
		mStride = stride;
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		if (mBufferType == BufferType::Constant) {
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
			desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
			// Constant buffer needs to be 16 byte aligned
			desc.ByteWidth = mStride + (16 - mStride % 16);
			desc.StructureByteStride = 0;
		}
		else {
			desc.BindFlags = mBufferType == BufferType::Vertex ?
				D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER : D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
			desc.ByteWidth = mStride * mCount;
		}
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA bufferData;
		ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		bufferData.pSysMem = data;

		D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateBuffer(&desc, &bufferData, mBuffer.GetAddressOf()));
	}

	BufferType BufferD3D11::GetBufferType() const
	{
		return mBufferType;
	}

	ID3D11Buffer* BufferD3D11::GetBufferD3D() const
	{
		return mBuffer.Get();
	}
}
#endif
