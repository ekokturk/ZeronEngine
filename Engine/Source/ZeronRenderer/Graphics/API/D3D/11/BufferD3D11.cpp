// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_D3D
#include "BufferD3D11.h"
#include "Graphics/API/D3D/11/GraphicsD3D11.h"
#include "Graphics/API/D3D/DebugInfoD3D.h"

namespace Zeron {
	
	VertexBufferD3D11::VertexBufferD3D11(GraphicsD3D11& graphics, const std::vector<Vertex>& data)
	{
		ZE_ASSERT(!data.empty(), "Can't have empty vertex buffer");
		mSize = static_cast<UINT>(data.size());
		// TODO: Parameterize these
		mStride = sizeof(Vertex);
		mOffset = 0;
		
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = mStride * mSize;
		bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA bufferData;
		ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		bufferData.pSysMem = &data.front();

		D3D_ASSERT_RESULT(graphics.GetDevice()->CreateBuffer(&bufferDesc, &bufferData, mBuffer.GetAddressOf()));
	}

	ID3D11Buffer* VertexBufferD3D11::GetBuffer() const
	{
		return mBuffer.Get();
	}

	void VertexBufferD3D11::BindBuffer(GraphicsD3D11& graphics) const
	{
		graphics.GetDeviceContext()->IASetVertexBuffers(0, 1, mBuffer.GetAddressOf(), &mStride, &mOffset);
	}
}
#endif
