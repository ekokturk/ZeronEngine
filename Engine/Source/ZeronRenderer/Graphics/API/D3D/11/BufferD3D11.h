// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#if ZE_GRAPHICS_D3D
#include "Graphics/API/D3D/DXGI.h"
#include "Graphics/GraphicsTypes.h"
#include "Graphics/Buffer.h"

struct ID3D11Buffer;

namespace Zeron
{
	class GraphicsD3D11;

	class BufferD3D11 : public Buffer {
	public:
		BufferD3D11(GraphicsD3D11& graphics, BufferType type, const void* data, uint32_t size, uint32_t stride);
		
		BufferType GetBufferType() const override;
		ID3D11Buffer* GetBufferD3D() const;
	
	protected:
		ZE::ComPtr<ID3D11Buffer> mBuffer;
		BufferType mBufferType;
	};
}
#endif
