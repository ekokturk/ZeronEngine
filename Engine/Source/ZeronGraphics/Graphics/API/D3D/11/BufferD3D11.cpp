// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_D3D
#include <Graphics/API/D3D/11/BufferD3D11.h>

#include <d3d11.h>
#include <Graphics/API/D3D/DebugInfoD3D.h>
#include <Graphics/API/D3D/11/D3D11Helpers.h>
#include <Graphics/API/D3D/11/GraphicsD3D11.h>

namespace Zeron
{
	BufferD3D11::BufferD3D11(GraphicsD3D11& graphics, BufferType type, uint32_t count, uint32_t stride, const void* data, BufferUsageType usage)
		: Buffer(type, count, stride, usage)
		, mMappedMemory(nullptr)
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.BindFlags = D3D11Helpers::GetBufferType(type);
		desc.Usage = _getUsageFlagD3D();
		desc.CPUAccessFlags = _getCpuAccessFlagD3D();
		desc.MiscFlags = 0;
		if (GetBufferType() == BufferType::Uniform) {
			// Constant buffer needs to be 16 byte aligned
			desc.ByteWidth = GetStride() + (16 - GetStride() % 16);
			desc.StructureByteStride = 0;
		}
		else {
			desc.ByteWidth = GetSizeInBytes();
		}

		D3D11_SUBRESOURCE_DATA bufferData;
		ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		bufferData.pSysMem = data;

		ZE_D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateBuffer(&desc, data ? &bufferData : nullptr, mBuffer.GetAddressOf()));
	}

	void BufferD3D11::MapD3D(ID3D11DeviceContext* device)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZE_D3D_ASSERT_RESULT(device->Map(mBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		mMappedMemory = mappedResource.pData;
	}

	void BufferD3D11::UnMapD3D(ID3D11DeviceContext* device)
	{
		ZE_D3D_ASSERT(device->Unmap(mBuffer.Get(), 0));
		mMappedMemory = nullptr;
	}

	void BufferD3D11::UpdateD3D(ID3D11DeviceContext* device, const void* data, uint32_t sizeBytes, uint32_t offset, BufferUpdateRule updateRule)
	{
		ZE_ASSERT((GetCount() - offset) * GetStride() >= sizeBytes, "D3D11 buffer update of {} bytes at {} overflowed!", sizeBytes, offset);
		ZE_ASSERT(GetUsageType() == BufferUsageType::Dynamic || GetUsageType() == BufferUsageType::Staging, "D3D11 buffer memory is not accessible by CPU!");
		if (!mMappedMemory) {
			MapD3D(device);
		}
		const uint32_t memOffset = GetStride() * offset;
		unsigned char* offsetMemory = static_cast<unsigned char*>(mMappedMemory);
		if (mMappedMemory) {
			CopyMemory(&offsetMemory[memOffset], data, sizeBytes);
			if (updateRule == BufferUpdateRule::UnmapMemory || updateRule == BufferUpdateRule::KeepMappedMemoryIfAllowed) {
				UnMapD3D(device);
			}
		}
	}

	ID3D11Buffer* BufferD3D11::GetBufferD3D() const
	{
		return mBuffer.Get();
	}

	D3D11_USAGE BufferD3D11::_getUsageFlagD3D() const
	{
		switch (GetUsageType()) {
			case BufferUsageType::Default: return D3D11_USAGE::D3D11_USAGE_DEFAULT;
			case BufferUsageType::Staging: return D3D11_USAGE::D3D11_USAGE_STAGING;
			case BufferUsageType::Dynamic:  return D3D11_USAGE::D3D11_USAGE_DYNAMIC;
			case BufferUsageType::Immutable: return D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
			default: ZE_FAIL("D3D11 buffer usage type is not supported!");
		}
		return D3D11_USAGE::D3D11_USAGE_DEFAULT;
	}

	UINT BufferD3D11::_getCpuAccessFlagD3D() const
	{
		switch (GetUsageType()) {
			case BufferUsageType::Default: return 0;
			case BufferUsageType::Dynamic: return D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
			case BufferUsageType::Staging:
			case BufferUsageType::Immutable:;
			default: ZE_FAIL("D3D11 buffer usage type is not supported!");
		}
		return 0;
	}
}
#endif
