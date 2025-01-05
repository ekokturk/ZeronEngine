// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/DXGI.h>
#	include <Graphics/Buffer.h>
#	include <Graphics/GraphicsTypes.h>

struct ID3D11Buffer;
struct ID3D11DeviceContext;
struct D3D11_MAPPED_SUBRESOURCE;
struct ID3D11ShaderResourceView;
struct ID3D11UnorderedAccessView;
enum D3D11_USAGE;
enum D3D11_BIND_FLAG;

namespace Zeron::Gfx
{
	class GraphicsD3D11;

	class BufferD3D11 : public Buffer {
	  public:
		BufferD3D11(GraphicsD3D11& graphics, BufferType type, uint32_t count, uint32_t stride, const void* data, BufferUsageType usage);

		void SetDebugName(std::string_view label) override;

		// D3D11 API
		void MapD3D(ID3D11DeviceContext* device);
		void UnMapD3D(ID3D11DeviceContext* device);
		void UpdateD3D(ID3D11DeviceContext* device, const void* data, uint32_t sizeBytes, uint32_t offset, BufferUpdateRule updateRule);
		ID3D11Buffer* GetBufferD3D() const;
		ID3D11ShaderResourceView* GetResourceViewD3D() const;
		ID3D11UnorderedAccessView* GetUnorderedAccessViewD3D() const;

	  protected:
		UINT _getBindFlagD3D() const;
		D3D11_USAGE _getUsageFlagD3D() const;
		UINT _getCpuAccessFlagD3D() const;

		Gfx::ComPtr<ID3D11Buffer> mBuffer;
		Gfx::ComPtr<ID3D11ShaderResourceView> mResourceView;
		Gfx::ComPtr<ID3D11UnorderedAccessView> mUnorderedAccessView;
		void* mMappedMemory;
	};
}
#endif