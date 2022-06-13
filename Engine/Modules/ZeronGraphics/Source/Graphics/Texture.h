// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>

namespace Zeron
{
	class Texture {
		
	protected:
		static constexpr const char* RESOURCE_PATH = "Resources/Textures/";

	public:
		Texture(TextureType type);
		virtual ~Texture() = default;

		TextureType GetTextureType() const;
	
	private:
		TextureType mType = TextureType::Invalid;
	};
}
