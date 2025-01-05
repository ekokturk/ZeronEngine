// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>

namespace Zeron::Gfx
{
	class Texture {
	  public:
		Texture(const Vec2i& extent, TextureType type, TextureFormat format, MSAALevel sampling);
		virtual ~Texture() = default;

		const Vec2i& GetExtent() const;
		uint32_t GetCount() const;
		TextureFormat GetFormat() const;
		uint32_t GetStride() const;
		TextureType GetType() const;
		MSAALevel GetSampling() const;

	  private:
		TextureType mType;
		TextureFormat mFormat;
		Vec2i mExtent;
		MSAALevel mSampling;
	};
}