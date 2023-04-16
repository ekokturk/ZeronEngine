// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>

namespace Zeron
{
	class Texture {
	  public:
		Texture(TextureType type);
		virtual ~Texture() = default;

		TextureType GetTextureType() const;

	  private:
		TextureType mType = TextureType::Invalid;
	};
}