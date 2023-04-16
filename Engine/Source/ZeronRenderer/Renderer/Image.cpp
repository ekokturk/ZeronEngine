// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Renderer/Image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Zeron
{
	Image::Image()
		: mColorChannel(ColorChannel::None)
		, mRawData(nullptr)
	{
	}

	Image::~Image()
	{
		Clear();
	}

	bool Image::Load(const ByteBuffer& buffer, ColorChannel channel, bool storeRawData)
	{
		const int desiredChannel = static_cast<int>(channel);
		if(desiredChannel > 4 || desiredChannel < 1) {
			ZE_FAIL("Requested invalid color channel");
			return false;
		}
		
		Clear();
		int fileChannels = 0;
		stbi_uc* data = stbi_load_from_memory(reinterpret_cast<stbi_uc const*>(buffer.data()), buffer.size(), &mSize.X, &mSize.Y, &fileChannels, desiredChannel);
		ZE_ASSERT(fileChannels >= desiredChannel, "Image does not contain the desired color channel!");
		
		if(!data) {
			ZE_LOGE("Unable to load image data");
			return false;
		}

		// We either use raw byte data or RGBA CSolor for the loaded image
		mColorChannel = channel;
		if(storeRawData) {
			mRawData = data;
		}
		else {
			mData.reserve(mSize.X * mSize.Y * desiredChannel);
			for (int j = 0; j < mSize.Y; ++j) {
				for (int i = 0; i < mSize.X; ++i) {
					const int index = desiredChannel * i + desiredChannel * mSize.X * j;
					switch (mColorChannel) { 
						case ColorChannel::Gray: {
							mData.emplace_back(data[index], data[index], data[index]);
						} break;
						case ColorChannel::GrayA: {
							mData.emplace_back(data[index], data[index], data[index], data[index + 1]);
						} break;
						case ColorChannel::RGB: {
							mData.emplace_back(data[index], data[index + 1], data[index + 2]);
						} break;
						case ColorChannel::RGBA: {
							mData.emplace_back(data[index], data[index + 1], data[index + 2], data[index + 3]);
						} break;
						case ColorChannel::None:
						default: ZE_FAIL("Invalid color channel!");
					}
				}
			}
			stbi_image_free(data);
		}
		return true;
	}

	void Image::Clear()
	{
		mSize = Vec2i::ZERO;
		mColorChannel = ColorChannel::None;
		mData.clear();
		if (mRawData) {
			stbi_image_free(mRawData);
		}
	}

	const std::vector<Color>& Image::GetColorData() const
	{
		ZE_ASSERT(!mRawData, "Can't use Zeron::Color data when image stores raw data!");
		return mData;
	}

	const unsigned char* Image::GetRawData() const
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

	int Image::GetByteSize() const
	{
		return mRawData ?
			mSize.X * mSize.Y * static_cast<int>(mColorChannel) :
			static_cast<int>(mData.size()) * 4;
	}
}
