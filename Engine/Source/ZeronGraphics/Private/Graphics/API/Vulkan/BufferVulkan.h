// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanCommon.h>

#	include <Graphics/Buffer.h>
#	include <Graphics/GraphicsTypes.h>

namespace Zeron::Gfx
{
	class GraphicsVulkan;

	class BufferVulkan final : public Buffer {
	  public:
		BufferVulkan(GraphicsVulkan& graphics, BufferType type, uint32_t count, uint32_t stride, const void* data, BufferUsageType usage);

		void SetDebugName(std::string_view label) override;

		// Vulkan API
		void MapVK(const vk::Device& device);
		void UnMapVK(const vk::Device& device);
		void UpdateVK(const vk::Device& device, const void* data, uint32_t sizeBytes, uint32_t offset, BufferUpdateRule updateRule);
		vk::Buffer& GetBufferVK();

	  private:
		vk::BufferUsageFlagBits _getUsageFlagVK() const;
		vk::MemoryPropertyFlags _getMemoryFlagVK() const;

		vk::UniqueBuffer mBuffer;
		vk::UniqueDeviceMemory mBufferMemory;
		void* mMappedMemory;

#	if ZE_DEBUG
		class VulkanObjectDebugInterface* mDebugInterface = nullptr;
#	endif
	};
}
#endif