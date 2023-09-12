// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	namespace Asset
	{
		class Image {
		  public:
			Image(const ByteBuffer& buffer, ColorChannel channel = ColorChannel::RGBA);
			~Image();

			std::vector<Color> GetAsColor() const;
			std::span<Color> GetRawColor() const;
			unsigned char* GetRawData() const;
			bool IsValid() const;

			int GetWidth() const;
			int GetHeight() const;
			int GetSize() const;
			int GetByteSize() const;
			ColorChannel GetColorChannel() const;

		  private:
			unsigned char* mRawData;
			ColorChannel mColorChannel;
			Vec2i mSize;
		};
	}
}