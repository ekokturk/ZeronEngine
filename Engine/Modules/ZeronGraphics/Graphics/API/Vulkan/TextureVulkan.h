// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN
#include <Graphics/API/Vulkan/VulkanCommon.h>

#include <Graphics/Texture.h>

namespace Zeron
{
	class GraphicsVulkan;

	class TextureVulkan final : public Texture {
	public:
		TextureVulkan(GraphicsVulkan& graphics, const Vec2i& size, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::SampleCountFlagBits sampling);
		TextureVulkan(vk::Image texture, const Vec2i& size, vk::Format format, vk::SampleCountFlagBits sampling);

		// TODO: Move this to base class
		const Vec2i& GetSize() const;

		// Vulkan
		vk::Image& GetImageVK();
		vk::Format GetFormatVK() const;
		uint32_t GetMipLevelVK() const;
		vk::ImageView GetImageViewVK() const;

		vk::UniqueImageView CreateImageView(const vk::Device& device, vk::ImageAspectFlags flags);

	private:
		vk::Image mImage;
		
		vk::UniqueImage mOwnedImage;
		vk::UniqueDeviceMemory mOwnedImageMemory;
		vk::UniqueImageView mImageView;

		Vec2i mSize;
		vk::Format mFormat = vk::Format::eUndefined;
		vk::SampleCountFlagBits mSampling = vk::SampleCountFlagBits::e1;
	};

}
#endif