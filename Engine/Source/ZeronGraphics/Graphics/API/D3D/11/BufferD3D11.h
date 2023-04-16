// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/DXGI.h>
#	include <Graphics/Buffer.h>
#	include <Graphics/GraphicsTypes.h>

struct ID3D11Buffer;
struct ID3D11DeviceContext;
struct D3D11_MAPPED_SUBRESOURCE;
enum D3D11_USAGE;

namespace Zeron
{
	class GraphicsD3D11;

	class BufferD3D11 : public Buffer {
	  public:
		BufferD3D11(GraphicsD3D11& graphics, BufferType type, uint32_t count, uint32_t stride, const void* data, BufferUsageType usage);

		// D3D11 API
		void MapD3D(ID3D11DeviceContext* device);
		void UnMapD3D(ID3D11DeviceContext* device);
		void UpdateD3D(ID3D11DeviceContext* device, const void* data, uint32_t sizeBytes, uint32_t offset, BufferUpdateRule updateRule);
		ID3D11Buffer* GetBufferD3D() const;

	  protected:
		D3D11_USAGE _getUsageFlagD3D() const;
		UINT _getCpuAccessFlagD3D() const;

		ZE::ComPtr<ID3D11Buffer> mBuffer;
		void* mMappedMemory;
	};
}
#endif