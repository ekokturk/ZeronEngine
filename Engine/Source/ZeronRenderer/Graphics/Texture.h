// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "GraphicsTypes.h"

namespace Zeron
{
	class Texture {
		
	protected:
		static constexpr const char* RESOURCE_PATH = "Resources/Textures/";

	public:
		virtual ~Texture() = default;

		[[nodiscard]] TextureType GetTextureType() const;
	
	protected:
		TextureType mType = TextureType::Invalid;
	};
}
