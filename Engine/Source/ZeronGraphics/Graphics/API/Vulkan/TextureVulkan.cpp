// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN
#include <Graphics/API/Vulkan/TextureVulkan.h>

#include <Graphics/API/Vulkan/GraphicsVulkan.h>

namespace Zeron
{
	TextureVulkan::TextureVulkan(GraphicsVulkan& graphics, const Vec2i& size, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::SampleCountFlagBits sampling)
		// TODO: Implement generic texture types
		: Texture(TextureType::Invalid)
		, mSize(size)
		, mFormat(format)
		, mSampling(sampling)
	{
		const vk::Device& device = graphics.GetDeviceVK();

		// Create Image
		const vk::Extent3D extent(size.X, size.Y, 1);
		const vk::ImageCreateInfo imageCreateInfo(
			vk::ImageCreateFlags(),
			vk::ImageType::e2D,
			format,
			extent,
			GetMipLevelVK(),
			1,
			mSampling,
			tiling,
			usage
		);
		mOwnedImage = device.createImageUnique(imageCreateInfo);
		mImage = *mOwnedImage;

		// Allocate Image memory
		const vk::MemoryRequirements memoryRequirements = device.getImageMemoryRequirements(*mOwnedImage);
		const uint32_t memoryTypeIndex = graphics.GetMemoryTypeIndexVK(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
		const vk::MemoryAllocateInfo info(
			memoryRequirements.size,
			memoryTypeIndex
		);
		mOwnedImageMemory = device.allocateMemoryUnique(info);
		device.bindImageMemory(*mOwnedImage, mOwnedImageMemory.get(), 0);

		// Only create default resources for sampled textures
		if((usage & vk::ImageUsageFlagBits::eSampled) == vk::ImageUsageFlagBits::eSampled) {
			mImageView = CreateImageView(device, vk::ImageAspectFlagBits::eColor);
		}
	}

	TextureVulkan::TextureVulkan(vk::Image texture, const Vec2i& size, vk::Format format, vk::SampleCountFlagBits sampling)
		: Texture(TextureType::Invalid)
		, mImage(texture)
		, mSize(size)
		, mFormat(format)
		, mSampling(sampling)
	{
		ZE_ASSERT(mImage, "Vulkan texture cannot be null!");
	}

	const Vec2i& TextureVulkan::GetSize() const
	{
		return mSize;
	}

	vk::Image& TextureVulkan::GetImageVK()
	{
		return mImage;
	}

	vk::Format TextureVulkan::GetFormatVK() const
	{
		return mFormat;
	}

	uint32_t TextureVulkan::GetMipLevelVK() const
	{
		return 1;
	}

	vk::ImageView TextureVulkan::GetImageViewVK() const
	{
		ZE_ASSERT(mImageView, "Vulkan texture default image view is not found!");
		return *mImageView;
	}

	vk::UniqueImageView TextureVulkan::CreateImageView(const vk::Device& device, vk::ImageAspectFlags flags)
	{
		const vk::ImageSubresourceRange rangeInfo(
			flags,
			0,
			GetMipLevelVK(),
			0,
			1
		);

		const vk::ImageViewCreateInfo createInfo( 
			vk::ImageViewCreateFlags(),
			GetImageVK(),
			vk::ImageViewType::e2D,
			GetFormatVK(),
			vk::ComponentMapping(),
			rangeInfo
		);

		return device.createImageViewUnique(createInfo);
	}

}

#endif

