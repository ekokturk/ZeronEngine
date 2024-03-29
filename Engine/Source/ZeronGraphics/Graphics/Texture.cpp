// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/Texture.h>

namespace Zeron::Gfx
{
	Texture::Texture(TextureType type)
		: mType(type)
	{}

	TextureType Texture::GetTextureType() const
	{
		return mType;
	}
}