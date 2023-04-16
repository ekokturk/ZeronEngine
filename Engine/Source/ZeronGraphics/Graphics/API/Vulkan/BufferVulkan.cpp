// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/BufferVulkan.h>

#	include <Graphics/API/Vulkan/GraphicsVulkan.h>
#	include <Graphics/API/Vulkan/VulkanHelpers.h>

namespace Zeron
{
	BufferVulkan::BufferVulkan(GraphicsVulkan& graphics, BufferType type, uint32_t count, uint32_t stride, const void* data, BufferUsageType usage)
		: Buffer(type, count, stride, usage)
		, mMappedMemory(nullptr)
	{
		const vk::Device& device = graphics.GetDeviceVK();
		const vk::PhysicalDevice& physicalDevice = graphics.GetPrimaryAdapterVK();

		const vk::BufferUsageFlagBits typeFlag = VulkanHelpers::GetBufferType(GetBufferType());
		const vk::BufferUsageFlagBits usageFlag = _getUsageFlagVK();
		const vk::BufferCreateInfo bufferInfo(vk::BufferCreateFlags(), GetSizeInBytes(), typeFlag | usageFlag, vk::SharingMode::eExclusive, 0, nullptr);
		mBuffer = device.createBufferUnique(bufferInfo);

		const vk::MemoryPropertyFlags memoryProps = _getMemoryFlagVK();
		const vk::MemoryRequirements memoryRequirements = device.getBufferMemoryRequirements(*mBuffer);
		const uint32_t memoryTypeIndex = graphics.GetMemoryTypeIndexVK(memoryRequirements.memoryTypeBits, memoryProps);
		const vk::MemoryAllocateInfo memoryInfo(memoryRequirements.size, memoryTypeIndex);
		mBufferMemory = device.allocateMemoryUnique(memoryInfo);

		device.bindBufferMemory(mBuffer.get(), mBufferMemory.get(), 0);

		if (data) {
			UpdateVK(device, data, GetSizeInBytes(), 0, BufferUpdateRule::UnmapMemory);
		}
	}

	void BufferVulkan::MapVK(const vk::Device& device)
	{
		mMappedMemory = device.mapMemory(mBufferMemory.get(), 0, GetSizeInBytes());
	}

	void BufferVulkan::UnMapVK(const vk::Device& device)
	{
		device.unmapMemory(mBufferMemory.get());
		mMappedMemory = nullptr;
	}

	void BufferVulkan::UpdateVK(const vk::Device& device, const void* data, uint32_t sizeBytes, uint32_t offset, BufferUpdateRule updateRule)
	{
		ZE_ASSERT((GetCount() - offset) * GetStride() >= sizeBytes, "Vulkan buffer update of {} bytes at {} overflowed!", sizeBytes, offset);
		ZE_ASSERT(GetUsageType() == BufferUsageType::Dynamic || GetUsageType() == BufferUsageType::Staging, "Vulkan buffer memory is not accessible by CPU!");
		if (!mMappedMemory) {
			MapVK(device);
		}
		const uint32_t memOffset = GetStride() * offset;
		auto offsetMemory = static_cast<unsigned char*>(mMappedMemory);
		if (mMappedMemory) {
			std::memcpy(&offsetMemory[memOffset], data, sizeBytes);
			if (updateRule == BufferUpdateRule::UnmapMemory) {
				UnMapVK(device);
			}
		}
	}

	vk::Buffer& BufferVulkan::GetBufferVK()
	{
		return *mBuffer;
	}

	vk::MemoryPropertyFlags BufferVulkan::_getMemoryFlagVK() const
	{
		switch (GetUsageType()) {
			case BufferUsageType::Default: return vk::MemoryPropertyFlagBits::eDeviceLocal;
			case BufferUsageType::Staging: return vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
			case BufferUsageType::Dynamic: return vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
			case BufferUsageType::Immutable:;
			default: ZE_FAIL("Vulkan buffer doesn't support usage type!");
		}
	}

	vk::BufferUsageFlagBits BufferVulkan::_getUsageFlagVK() const
	{
		switch (GetUsageType()) {
			case BufferUsageType::Default: return vk::BufferUsageFlagBits::eTransferDst;
			case BufferUsageType::Staging: return vk::BufferUsageFlagBits::eTransferSrc;
			case BufferUsageType::Dynamic: return vk::BufferUsageFlagBits::eTransferDst;
			case BufferUsageType::Immutable:;
			default: ZE_FAIL("Vulkan buffer doesn't support usage type!");
		}
	}
}
#endif