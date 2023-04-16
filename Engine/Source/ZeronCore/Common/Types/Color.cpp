// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Common/Types/Color.h>

#include <fmt/format.h>

namespace Zeron
{
	Color::Color(Palette color)
		: mRed((static_cast<uint32_t>(color) >> 16) & 0xFF)
		, mGreen((static_cast<uint32_t>(color) >> 8) & 0xFF)
		, mBlue(static_cast<uint32_t>(color) & 0xFF)
		, mAlpha(255)
	{}

	Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: mRed(r)
		, mGreen(g)
		, mBlue(b)
		, mAlpha(a)
	{}

	Color::Color(float r, float g, float b, float a)
		: mRed(static_cast<uint8_t>(r * 255))
		, mGreen(static_cast<uint8_t>(g * 255))
		, mBlue(static_cast<uint8_t>(b * 255))
		, mAlpha(static_cast<uint8_t>(a * 255))
	{}

	Color::Color(uint32_t hex, float a)
		: mRed((hex >> 16) & 0xFF)
		, mGreen((hex >> 8) & 0xFF)
		, mBlue(hex & 0xFF)
		, mAlpha(static_cast<uint8_t>(a * 255))
	{}

	std::string Color::HexString() const
	{
		return fmt::format("0x{0:X}", HexRGB());
	}

	std::string Color::HexRGBString() const
	{
		return fmt::format("#{0:X}", HexRGB());
	}

	std::string Color::HexRGBAString() const
	{
		return fmt::format("#{0:X}", HexRGBA());
	}

	std::string Color::HexARGBString() const
	{
		return fmt::format("#{0:X}", HexARGB());
	}
}