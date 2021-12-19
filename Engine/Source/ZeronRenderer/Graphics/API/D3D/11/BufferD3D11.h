// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#if ZE_GRAPHICS_D3D
#include "Graphics/Buffer.h"
#include "Graphics/Primitives.h"
#include <wrl/client.h>
#include <d3d11.h>

struct ID3D11Buffer;

namespace Zeron {
	class GraphicsD3D11;

	class VertexBufferD3D11 : public Buffer {
	public:
		VertexBufferD3D11(GraphicsD3D11& graphics, const std::vector<Vertex>& data);
		// TODO: Move to layout object
		UINT GetStride() const { return mStride; }
		UINT GetOffset() const { return mOffset; }
		UINT GetSize() const { return mSize; }

		ID3D11Buffer* GetBuffer() const;
		void BindBuffer(GraphicsD3D11& graphics) const;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
		UINT mSize = 0;
		UINT mOffset = 0;
		UINT mStride = 0;
	};
}
#endif
