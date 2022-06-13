// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	class Image {
	public:
		enum class Channels {
			Gray = 1,
			GrayA = 2,
			RGB = 3,
			RGBA = 4
		};
		
		Image();
		Image(const std::string& path);
		~Image();

		bool Load(const std::string& path);

		const Color* GetColorData() const;
		int GetWidth() const;
		int GetHeight() const;

	private:
		Color* mData;
		Vec2i mSize;
	};
}


