// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>

namespace Zeron
{
	class GraphicsContext;
	class Shader;
	class Texture;
	
	class Material {
	public:
		
		struct Properties {
			bool mIsTransparent;
		};

		void Bind(GraphicsContext& context);

		//bool AddTexture(std::shared_ptr<Texture> texture);
		//bool RemoveTexture(TextureType texture);

	private:
		std::shared_ptr<Shader> mShader;
		std::unordered_map<TextureType, std::shared_ptr<Texture>> mTextures;
	};
}


