// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/TextureVulkan.h>

#	include <Graphics/API/Vulkan/GraphicsVulkan.h>
#	include <Graphics/API/Vulkan/VulkanHelpers.h>

namespace Zeron::Gfx
{
	TextureVulkan::TextureVulkan(GraphicsVulkan& graphics, const Vec2i& size, TextureType type, TextureFormat format, MSAALevel sampling, vk::ImageUsageFlags usage)
		: Texture(size, type, format, sampling)
	{
		const vk::Device& device = graphics.GetDeviceVK();

		// Create Image
		const vk::Extent3D extent(size.X, size.Y, 1);
		const vk::ImageCreateInfo imageCreateInfo(
			vk::ImageCreateFlags(),
			vk::ImageType::e2D,
			GetFormatVK(),
			extent,
			GetMipLevelVK(),
			1,
			VulkanHelpers::GetMultiSamplingLevel(GetSampling()),
			vk::ImageTiling::eOptimal,
			usage
		);
		mOwnedImage = device.createImageUnique(imageCreateInfo);
		mImage = *mOwnedImage;

		// Allocate Image memory
		const vk::MemoryRequirements memoryRequirements = device.getImageMemoryRequirements(*mOwnedImage);
		const uint32_t memoryTypeIndex = graphics.GetMemoryTypeIndexVK(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
		const vk::MemoryAllocateInfo info(memoryRequirements.size, memoryTypeIndex);
		mOwnedImageMemory = device.allocateMemoryUnique(info);
		device.bindImageMemory(*mOwnedImage, mOwnedImageMemory.get(), 0);
	}

	TextureVulkan::TextureVulkan(vk::Image texture, const Vec2i& size, TextureType type, TextureFormat format, MSAALevel sampling)
		: Texture(size, type, format, sampling)
		, mImage(texture)
	{
		ZE_ASSERT(mImage, "Vulkan texture cannot be null!");
	}

	vk::Image& TextureVulkan::GetImageVK()
	{
		return mImage;
	}

	vk::Format TextureVulkan::GetFormatVK() const
	{
		return VulkanHelpers::GetTextureFormat(GetFormat());
	}

	uint32_t TextureVulkan::GetMipLevelVK() const
	{
		return 1;
	}

	vk::ImageView TextureVulkan::GetOrCreateImageViewVK(const vk::Device& device)
	{
		if (!mImageView) {
			if (GetType() == TextureType::Depth) {
				mImageView = CreateImageView(device, vk::ImageAspectFlagBits::eDepth);
			}
			else {
				mImageView = CreateImageView(device, vk::ImageAspectFlagBits::eColor);
			}
		}
		return mImageView.get();
	}

	vk::ImageView TextureVulkan::GetImageViewVK() const
	{
		ZE_ASSERT(mImageView, "Vulkan texture default image view is not found!");
		return *mImageView;
	}

	vk::UniqueImageView TextureVulkan::CreateImageView(const vk::Device& device, vk::ImageAspectFlags flags) const
	{
		const vk::ImageSubresourceRange rangeInfo(flags, 0, GetMipLevelVK(), 0, 1);
		const vk::ImageViewCreateInfo createInfo(vk::ImageViewCreateFlags(), mImage, vk::ImageViewType::e2D, GetFormatVK(), vk::ComponentMapping(), rangeInfo);
		return device.createImageViewUnique(createInfo);
	}

}

#endif