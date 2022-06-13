// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Renderer/Image.h>

#include <stb/stb_image.h>

namespace Zeron
{
	Image::Image()
		: mData(nullptr)
	{
	}

	Image::Image(const std::string& path)
		: mData(nullptr)
	{
		Load(path);
	}

	Image::~Image()
	{
		stbi_image_free(mData);
	}

	bool Image::Load(const std::string& path)
	{
		int channels = 0;
		stbi_uc* imageData = stbi_load(path.c_str(), &mSize.X, &mSize.Y, &channels, STBI_rgb_alpha);
		if (imageData && channels != 0) {
			// TODO: Support loading other channels
			// We can possibly use std::vector<Color> here for all channels
			
			mData = reinterpret_cast<Color*>(imageData);
			return true;
		}
		
		ZE_LOGE("Unable to load image '%s'", path.c_str());
		return false;
	}

	const Color* Image::GetColorData() const
	{
		return mData;
	}

	int Image::GetWidth() const
	{
		return mSize.X;
	}

	int Image::GetHeight() const
	{
		return mSize.Y;
	}
}
