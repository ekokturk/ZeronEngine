// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron {

	class Color {
	public:
		enum Palette {
			White = 0xFFFFFF,
			Black = 0x000000,
			Red = 0xFF0000,
			Green = 0x008000,
			Yellow = 0xFFFF00,
			Orange = 0xFFA500,
			Purple = 0x800080,
			Gray = 0x808080,
			DarkRed = 0x6E0000,
			LightBlue = 0x00FFFF
		};

		Color(Palette color);
		Color(uint8_t r, uint8_t g, uint8_t b, float a = 1.f);
		Color(float r, float g, float b, float a = 1.f);
		Color(uint32_t hex, float a = 1.f);

		Color& operator=(const Color&) = default;

		Color& operator=(uint32_t value) {
			mRed	= (value >> 16) & 0xFF;
			mGreen	= (value >> 8) & 0xFF;
			mBlue	=  value & 0xFF;
			mAlpha	=  1.f;
			return *this;
		}

		Color& operator=(Palette color) {
			*this = static_cast<uint32_t>(color);
			return *this;
		}

		[[nodiscard]] uint8_t R() const			{ return mRed;   }
		[[nodiscard]] uint8_t G() const			{ return mGreen; }
		[[nodiscard]] uint8_t B() const			{ return mBlue;  }
		[[nodiscard]] float	A() const			{ return mAlpha; }

		[[nodiscard]] float	normR() const { return static_cast<float>(mRed)	  / 255; }
		[[nodiscard]] float	normG() const { return static_cast<float>(mGreen) / 255; }
		[[nodiscard]] float	normB() const { return static_cast<float>(mBlue)  / 255; }
		
		[[nodiscard]] uint32_t HexRGB() const	{ return HexFromRGB(mRed, mGreen, mBlue); }
		[[nodiscard]] uint32_t HexRGBA() const	{ return HexFromRGBA(mRed, mGreen, mBlue, mAlpha); }
		[[nodiscard]] uint32_t HexARGB() const	{ return HexFromARGB(mRed, mGreen, mBlue, mAlpha); }

		[[nodiscard]] std::string HexString() const;
		[[nodiscard]] std::string HexRGBString() const;
		[[nodiscard]] std::string HexRGBAString() const;
		[[nodiscard]] std::string HexARGBString() const;


		static uint32_t HexFromRGB(uint8_t r, uint8_t g, uint8_t b) {
			return r << 16 | g << 8 | b;
		}

		static uint32_t HexFromRGB(float r, float g, float b) {
			return	static_cast<uint8_t>(r * 255) << 16 | 
					static_cast<uint8_t>(g * 255) << 8	| 
					static_cast<uint8_t>(b * 255);
		}

		static uint32_t HexFromARGB(uint8_t r, uint8_t g, uint8_t b, float a) {
			return	static_cast<uint8_t>(std::clamp(a, 0.f, 1.f) * 255) << 24 
					| HexFromRGB(r, g, b);
		}

		static uint32_t HexFromARGB(float r, float g, float b, float a) {
			return	static_cast<uint8_t>(std::clamp(a, 0.f, 1.f) * 255) << 24 
					| HexFromRGB( std::clamp(r, 0.f, 1.f), std::clamp(g, 0.f, 1.f),
								  std::clamp(b, 0.f, 1.f));
		}

		static uint32_t HexFromRGBA(uint8_t r, uint8_t g, uint8_t b, float a) {
			return	HexFromRGB(r, g, b) << 8
					| static_cast<uint8_t>(std::clamp(a, 0.f, 1.f) * 255);
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
		float		mAlpha;
	};

}


