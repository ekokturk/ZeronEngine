// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/Texture.h>

namespace Zeron::Gfx
{
	Texture::Texture(const Vec2i& extent, TextureType type, TextureFormat format, MSAALevel sampling)
		: mType(type)
		, mFormat(format)
		, mExtent(extent)
		, mSampling(sampling)
	{}

	uint32_t Texture::GetStride() const
	{
		switch (mFormat) {
			case TextureFormat::RGB_8U: {
				return 3;
			}
			case TextureFormat::RGBA_8U:
			case TextureFormat::BGRA_8U:
			case TextureFormat::Depth_32F:
			case TextureFormat::DepthStencil_32U: {
				return 4;
			}
			default: ZE_FAIL("Texture format is not supported");
		}
		return 0;
	}

	TextureType Texture::GetType() const
	{
		return mType;
	}

	MSAALevel Texture::GetSampling() const
	{
		return mSampling;
	}

	const Vec2i& Texture::GetExtent() const
	{
		return mExtent;
	}

	uint32_t Texture::GetCount() const
	{
		return mExtent.X * mExtent.Y;
	}

	TextureFormat Texture::GetFormat() const
	{
		return mFormat;
	}
}