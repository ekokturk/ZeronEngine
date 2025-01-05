// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once
#include <Render/Mesh/MeshData.h>

namespace Zeron
{
	namespace Gfx
	{
		class Graphics;
		class Shader;
		class Texture;
	}

	namespace Render
	{
		struct FontGlyph {
			std::array<Vec2, 4> mPositions;
			std::array<Vec2, 4> mUVs;
			Vec2 mOffset;
		};

		struct FontAtlasProps {
			Vec2i mAtlasSize = Vec2i(1024);
			float mFontSize = 40.f;
			Vec2i mOversampling = 2;
			int mFirstChar = ' ';
			int mCharCount = '~' - ' ';
		};

		class FontAtlas {
		  public:
			FontAtlas(std::string id, const FontAtlasProps& atlasProps = {});
			~FontAtlas();

			bool Load(Gfx::Graphics& graphics, const ByteBuffer& fontData);

			MeshData GenerateText(const std::string& text, bool useBottomBaseline = false) const;
			FontGlyph GenerateGlyph(char character, float offsetX, float offsetY) const;

			Gfx::Texture* GetTexture() const;

		  private:
			const std::string mId;
			const FontAtlasProps mProps;
			std::unique_ptr<Gfx::Texture> mAtlasTexture;
			ByteBuffer mCharInfoBuffer;
		};
	}
}
