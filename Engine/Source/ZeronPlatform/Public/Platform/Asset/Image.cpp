// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Platform/Asset/Image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Zeron::Asset
{
	Image::Image(const ByteBuffer& buffer, ColorChannel channel)
		: mRawData(nullptr)
		, mColorChannel(channel)
	{
		const int desiredChannel = static_cast<int>(channel);
		ZE_ASSERT(desiredChannel <= 4 && desiredChannel > 0, "Requested invalid color channel");
		int fileChannels = 0;
		stbi_uc* data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(buffer.data()), buffer.size(), &mSize.X, &mSize.Y, &fileChannels, desiredChannel);
		ZE_ASSERT(data, "Unable to load image data!");
		ZE_ASSERT(fileChannels >= desiredChannel, "Image does not contain the desired color channel!");
		mRawData = data;
	}

	Image::~Image()
	{
		if (mRawData) {
			stbi_image_free(mRawData);
		}
	}

	std::vector<Color> Image::GetAsColor() const
	{
		const int colorChannel = static_cast<int>(mColorChannel);
		const size_t bufferSize = mSize.X * mSize.Y * colorChannel;
		if (mColorChannel == ColorChannel::RGBA) {
			return { mRawData, mRawData + bufferSize };
		}
		std::vector<Color> data;
		data.reserve(bufferSize);
		for (int j = 0; j < mSize.Y; ++j) {
			for (int i = 0; i < mSize.X; ++i) {
				const int index = colorChannel * i + colorChannel * mSize.X * j;
				switch (mColorChannel) {
					case ColorChannel::Gray: {
						data.emplace_back(mRawData[index], mRawData[index], mRawData[index]);
					} break;
					case ColorChannel::GrayA: {
						data.emplace_back(mRawData[index], mRawData[index], mRawData[index], mRawData[index + 1]);
					} break;
					case ColorChannel::RGB: {
						data.emplace_back(mRawData[index], mRawData[index + 1], mRawData[index + 2]);
					} break;
					case ColorChannel::None:
					default: ZE_FAIL("Invalid color channel!");
				}
			}
		}
		return data;
	}

	std::span<Color> Image::GetRawColor() const
	{
		ZE_ASSERT(mColorChannel == ColorChannel::RGBA, "Raw color can only be used for RGBA!");
		return { reinterpret_cast<Color*>(mRawData), static_cast<size_t>(mSize.X * mSize.Y) };
	}

	unsigned char* Image::GetRawData() const
	{
		return mRawData;
	}

	bool Image::IsValid() const
	{
		return mRawData;
	}

	int Image::GetWidth() const
	{
		return mSize.X;
	}

	int Image::GetHeight() const
	{
		return mSize.Y;
	}

	int Image::GetSize() const
	{
		return mSize.X * mSize.Y;
	}

	int Image::GetByteSize() const
	{
		return mSize.X * mSize.Y * static_cast<int>(mColorChannel);
	}

	ColorChannel Image::GetColorChannel() const
	{
		return mColorChannel;
	}
}