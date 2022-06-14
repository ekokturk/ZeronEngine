// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron {

	enum class ColorChannel {
		None = 0,
		Gray = 1,
		GrayA = 2,
		RGB = 3,
		RGBA = 4
	};
	
	class Color {
	public:
		enum Palette {
			White = 0xFFFFFF,
			Black = 0x000000,
			LightGray = 0xD3D3D3,
			Gray = 0x808080,
			DarkGray = 0x696969,
			Yellow = 0xFFFF00,
			Orange = 0xFFA500,
			DarkOrange = 0xFF8C00,
			OrangeRed = 0xFF4500,
			Crimson = 0xDC143C,
			Red = 0xFF0000,
			DarkRed = 0x8B0000,
			Green = 0x008000,
			LightPink = 0xFFB6C1,
			Pink = 0xFF69B4,
			DarkPink = 0xFF1493,
			Purple = 0x800080,
			Violet = 0xEE82EE,
			SteelBlue = 0x4682B4,
			LightBlue = 0xADD8E6,
			Blue = 0x0000FF,
			DarkBlue = 0x00008B,
		};

		Color(Palette color);
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
		Color(float r, float g, float b, float a = 1.f);
		Color(uint32_t hex, float a = 1.f);

		Color& operator=(const Color&) = default;

		Color& operator=(uint32_t value) {
			mRed	= (value >> 16) & 0xFF;
			mGreen	= (value >> 8) & 0xFF;
			mBlue	=  value & 0xFF;
			mAlpha	=  255;
			return *this;
		}

		Color& operator=(Palette color) {
			*this = static_cast<uint32_t>(color);
			return *this;
		}

		uint8_t R() const			{ return mRed;   }
		uint8_t G() const			{ return mGreen; }
		uint8_t B() const			{ return mBlue;  }
		uint8_t A() const			{ return mAlpha; }

		float	normR() const { return static_cast<float>(mRed)	  / 255; }
		float	normG() const { return static_cast<float>(mGreen) / 255; }
		float	normB() const { return static_cast<float>(mBlue)  / 255; }
		float	normA() const { return static_cast<float>(mAlpha) / 255; }
		
		uint32_t HexRGB() const	{ return HexFromRGB(mRed, mGreen, mBlue); }
		uint32_t HexRGBA() const	{ return HexFromRGBA(mRed, mGreen, mBlue, mAlpha); }
		uint32_t HexARGB() const	{ return HexFromARGB(mRed, mGreen, mBlue, mAlpha); }

		std::string HexString() const;
		std::string HexRGBString() const;
		std::string HexRGBAString() const;
		std::string HexARGBString() const;


		static uint32_t HexFromRGB(uint8_t r, uint8_t g, uint8_t b) {
			return r << 16 | g << 8 | b;
		}

		static uint32_t HexFromRGB(float r, float g, float b) {
			return	static_cast<uint8_t>(r * 255) << 16 | 
					static_cast<uint8_t>(g * 255) << 8	| 
					static_cast<uint8_t>(b * 255);
		}

		static uint32_t HexFromARGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
			return	a << 24 | HexFromRGB(r, g, b);
		}

		static uint32_t HexFromARGB(float r, float g, float b, float a) {
			return	static_cast<uint8_t>(std::clamp(a, 0.f, 1.f) * 255) << 24 
					| HexFromRGB( std::clamp(r, 0.f, 1.f), std::clamp(g, 0.f, 1.f),
								  std::clamp(b, 0.f, 1.f));
		}

		static uint32_t HexFromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
			return	HexFromRGB(r, g, b) << 8 | a;
		}

		static uint32_t HexFromRGBA(float r, float g, float b, float a) {
			return	HexFromRGB(std::clamp(r, 0.f, 1.f), std::clamp(g, 0.f, 1.f),
							 std::clamp(b, 0.f, 1.f)) << 8 
					| static_cast<uint8_t>(std::clamp(a, 0.f, 1.f) * 255);
		}

	private:
		uint8_t		mRed;
		uint8_t		mGreen;
		uint8_t		mBlue;
		uint8_t		mAlpha;
	};

}


