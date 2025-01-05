// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/11/BufferD3D11.h>

#	include <d3d11.h>
#	include <Graphics/API/D3D/11/D3D11Helpers.h>
#	include <Graphics/API/D3D/11/GraphicsD3D11.h>
#	include <Graphics/API/D3D/DebugInfoD3D.h>

namespace Zeron::Gfx
{
	BufferD3D11::BufferD3D11(GraphicsD3D11& graphics, BufferType type, uint32_t count, uint32_t stride, const void* data, BufferUsageType usage)
		: Buffer(type, count, stride, usage)
		, mMappedMemory(nullptr)
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.BindFlags = _getBindFlagD3D();
		desc.Usage = _getUsageFlagD3D();
		desc.CPUAccessFlags = _getCpuAccessFlagD3D();
		desc.MiscFlags = GetBufferType() == BufferType::Storage ? D3D11_RESOURCE_MISC_BUFFER_STRUCTURED : 0;
		if (GetBufferType() == BufferType::Uniform) {
			// Constant buffer needs to be 16 byte aligned
			desc.ByteWidth = GetStride() + (16 - GetStride() % 16);
			desc.StructureByteStride = 0;
		}
		else {
			desc.ByteWidth = GetSizeInBytes();
			if (GetBufferType() == BufferType::Storage) {
				ZE_ASSERT(GetSizeInBytes() % 16 == 0, "BufferD3D11: Expected structured buffer (bytes: {}) to be 16-byte aligned", GetSizeInBytes());
				desc.StructureByteStride = GetStride();
			}
		}

		D3D11_SUBRESOURCE_DATA bufferData;
		ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		bufferData.pSysMem = data;

		ZE_D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateBuffer(&desc, data ? &bufferData : nullptr, mBuffer.GetAddressOf()));

		// We create views for structure buffers
		// TODO: This should be managed by the pipeline binding object instead
		if (GetBufferType() == BufferType::Storage) {
			if (GetUsageType() == BufferUsageType::Default) {
				D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
				uavDesc.Format = DXGI_FORMAT_UNKNOWN;
				uavDesc.Buffer.FirstElement = 0;
				uavDesc.Buffer.NumElements = GetCount();
				ZE_D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateUnorderedAccessView(mBuffer.Get(), &uavDesc, mUnorderedAccessView.GetAddressOf()));
			}
			else {
				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
				srvDesc.Format = DXGI_FORMAT_UNKNOWN;
				srvDesc.Buffer.FirstElement = 0;
				srvDesc.Buffer.NumElements = GetCount();
				ZE_D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateShaderResourceView(mBuffer.Get(), &srvDesc, mResourceView.GetAddressOf()));
			}
		}
	}

	void BufferD3D11::SetDebugName(std::string_view label)
	{
#	if ZE_DEBUG
		D3D_SET_OBJECT_NAME_N_A(mBuffer.Get(), label.size(), label.data());
#	endif
	}

	void BufferD3D11::MapD3D(ID3D11DeviceContext* device)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		// TODO: Support partial updates with D3D11_MAP_WRITE_NO_OVERWRITE but we have to ensure data is not being used by CPU
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
		std::byte* offsetMemory = static_cast<std::byte*>(mMappedMemory);
		if (mMappedMemory) {
			std::memcpy(&offsetMemory[memOffset], data, sizeBytes);
			if (updateRule == BufferUpdateRule::UnmapMemory || updateRule == BufferUpdateRule::KeepMappedMemoryIfAllowed) {
				UnMapD3D(device);
			}
		}
	}

	ID3D11Buffer* BufferD3D11::GetBufferD3D() const
	{
		return mBuffer.Get();
	}

	ID3D11ShaderResourceView* BufferD3D11::GetResourceViewD3D() const
	{
		return mResourceView.Get();
	}

	ID3D11UnorderedAccessView* BufferD3D11::GetUnorderedAccessViewD3D() const
	{
		return mUnorderedAccessView.Get();
	}

	UINT BufferD3D11::_getBindFlagD3D() const
	{
		UINT flag = D3D11Helpers::GetBufferType(GetBufferType());
		// If we didn't define storage buffer explicitly, it is used as unordered access
		if (GetBufferType() == BufferType::Storage && GetUsageType() == BufferUsageType::Default) {
			flag |= D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;
		}
		return flag;
	}

	D3D11_USAGE BufferD3D11::_getUsageFlagD3D() const
	{
		switch (GetUsageType()) {
			case BufferUsageType::Default: return D3D11_USAGE::D3D11_USAGE_DEFAULT;
			case BufferUsageType::Staging: return D3D11_USAGE::D3D11_USAGE_STAGING;
			case BufferUsageType::Dynamic: return D3D11_USAGE::D3D11_USAGE_DYNAMIC;
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