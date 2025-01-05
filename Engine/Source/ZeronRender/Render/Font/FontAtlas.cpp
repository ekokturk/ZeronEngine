// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Render/Font/FontAtlas.h>

#include <Graphics/Graphics.h>
#include <Graphics/Texture.h>
#include <Render/Mesh/MeshData.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

namespace Zeron::Render
{

	FontAtlas::FontAtlas(std::string id, const FontAtlasProps& atlasProps)
		: mId(std::move(id))
		, mProps(atlasProps)
	{}

	FontAtlas::~FontAtlas() = default;

	bool FontAtlas::Load(Gfx::Graphics& graphics, const ByteBuffer& fontData)
	{
		ZE_ASSERT(!mAtlasTexture, "Font Atlas is already loaded!");

		stbtt_fontinfo fontInfo;
		if (!stbtt_InitFont(&fontInfo, reinterpret_cast<const unsigned char*>(fontData.data()), 0)) {
			ZE_FAIL("Failed to initialize font");
			return false;
		}

		int ascent, descent, lineGap;
		stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);

		std::vector<uint8_t> atlasData(mProps.mAtlasSize.X * mProps.mAtlasSize.Y, 0);
		mCharInfoBuffer = ByteBuffer(mProps.mCharCount * sizeof(stbtt_packedchar));

		stbtt_pack_context context;
		if (!stbtt_PackBegin(&context, atlasData.data(), mProps.mAtlasSize.X, mProps.mAtlasSize.Y, 0, 1, nullptr)) {
			ZE_FAIL("Font Atlas failed to start packing font glyps!");
			return false;
		}

		stbtt_PackSetOversampling(&context, mProps.mOversampling.X, mProps.mOversampling.Y);
		if (!stbtt_PackFontRange(
				&context,
				reinterpret_cast<const unsigned char*>(fontData.data()),
				0,
				mProps.mFontSize,
				mProps.mFirstChar,
				mProps.mCharCount,
				mCharInfoBuffer.GetAs<stbtt_packedchar>()
			)) {
			ZE_FAIL("Font Atlas failed to pack font glyps!");
			return false;
		}

		stbtt_PackEnd(&context);

		// Create RGBA Texture
		// TODO: Support only RBG or Alpha texture
		std::vector<Color> buffer(mProps.mAtlasSize.X * mProps.mAtlasSize.Y, Color(0, 0, 0, 0.f));
		for (size_t i = 0; i < atlasData.size(); ++i) {
			buffer[i] = Color(255, 255, 255, atlasData[i]);
		}
		mAtlasTexture = graphics.CreateTexture(mProps.mAtlasSize, Gfx::TextureFormat::RGBA_8U, buffer.data());

		return true;
	}

	MeshData FontAtlas::GenerateText(const std::string& text, bool useBottomBaseline) const
	{
		struct Vertex {};
		MeshData mesh;
		if (!mCharInfoBuffer.empty()) {
			uint32_t lastIndex = 0;
			float offsetX = 0;
			// Vertices are generated from the bottom left corner, we need to offset them to top left to not use the bottom baseline
			float offsetY = useBottomBaseline ? 0.f : mProps.mFontSize;
			for (const char c : text) {
				const auto glyphInfo = GenerateGlyph(c, offsetX, offsetY);
				offsetX = glyphInfo.mOffset.X;
				offsetY = glyphInfo.mOffset.Y;
				// Skip vertices for empty characters
				if (c != ' ') {
					mesh.AppendVertices<MeshAttribute::ScreenPos>({ glyphInfo.mPositions });
					mesh.AppendVertices<MeshAttribute::UV>({ glyphInfo.mUVs });
					const std::array indices = { lastIndex, lastIndex + 1, lastIndex + 2, lastIndex, lastIndex + 2, lastIndex + 3 };
					mesh.AppendIndices(indices);
					lastIndex += 4;
				}
			}
		}

		return mesh;
	}

	FontGlyph FontAtlas::GenerateGlyph(char character, float offsetX, float offsetY) const
	{
		FontGlyph glyph{};
		if (!mCharInfoBuffer.empty()) {
			stbtt_aligned_quad quad;
			stbtt_GetPackedQuad(
				mCharInfoBuffer.GetAs<const stbtt_packedchar>(), mProps.mAtlasSize.X, mProps.mAtlasSize.Y, character - mProps.mFirstChar, &offsetX, &offsetY, &quad, 1
			);
			const float minX = quad.x0;
			const float maxX = quad.x1;
			// NOTE: Generated vertices will be inverted by default for orthographic projection
			// These Y values can be set to negative in order to support un-inverted text
			const float minY = quad.y1;
			const float maxY = quad.y0;

			glyph.mPositions = {
				Vec2{ minX, minY },
				Vec2{ minX, maxY },
				Vec2{ maxX, maxY },
				Vec2{ maxX, minY },
			};
			glyph.mUVs = {
				Vec2{ quad.s0, quad.t1 },
				Vec2{ quad.s0, quad.t0 },
				Vec2{ quad.s1, quad.t0 },
				Vec2{ quad.s1, quad.t1 },
			};
			glyph.mOffset = Vec2{ offsetX, offsetY };
		}

		return glyph;
	}

	Gfx::Texture* FontAtlas::GetTexture() const
	{
		return mAtlasTexture.get();
	}
}