// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Core/Types/Color.h>

namespace Zeron
{
	class Image {
	public:

		Image();
		~Image();

		bool Load(const std::string& path, ColorChannel channel = ColorChannel::RGBA, bool storeRawData = false);
		void Clear();

		const std::vector<Color>& GetColorData() const;
		const unsigned char* GetRawData() const;
		
		int GetWidth() const;
		int GetHeight() const;
		int GetByteSize() const;
		const std::string& GetFilePath() const;

	private:
		
		std::vector<Color> mData;
		unsigned char* mRawData;
		ColorChannel mColorChannel;
		Vec2i mSize;
		std::string mPath;
	};
}


