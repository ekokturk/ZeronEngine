// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>

namespace Zeron
{
	namespace Gfx
	{
		class GraphicsContext;
		class Shader;
		class Texture;
	}

	class Material {
	  public:
		struct Properties {
			bool mIsTransparent;
		};

		void Bind(Gfx::GraphicsContext& context);

		// bool AddTexture(std::shared_ptr<Texture> texture);
		// bool RemoveTexture(TextureType texture);

	  private:
		std::shared_ptr<Gfx::Shader> mShader;
		std::unordered_map<Gfx::TextureType, std::shared_ptr<Gfx::Texture>> mTextures;
	};
}