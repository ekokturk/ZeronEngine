// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanCommon.h>
#	include <Graphics/Texture.h>

namespace Zeron::Gfx
{
	class GraphicsVulkan;

	class TextureVulkan final : public Texture {
	  public:
		TextureVulkan(GraphicsVulkan& graphics, const Vec2i& size, TextureType type, TextureFormat format, MSAALevel sampling, vk::ImageUsageFlags usage);
		TextureVulkan(vk::Image texture, const Vec2i& size, TextureType type, TextureFormat format, MSAALevel sampling);

		// Vulkan
		vk::Image& GetImageVK();
		vk::Format GetFormatVK() const;
		uint32_t GetMipLevelVK() const;

		vk::ImageView GetOrCreateImageViewVK(const vk::Device& device);
		vk::ImageView GetImageViewVK() const;

		vk::UniqueImageView CreateImageView(const vk::Device& device, vk::ImageAspectFlags flags) const;

	  private:
		vk::Image mImage;
		vk::UniqueImage mOwnedImage;
		vk::UniqueDeviceMemory mOwnedImageMemory;
		vk::UniqueImageView mImageView;
	};

}
#endif