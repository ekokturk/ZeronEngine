// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::Gfx
{
	enum class VertexFormat
	{
		Unknown,
		Float2,
		Float3,
		Color
	};

	class VertexLayout {
	  public:
		struct Element {
			std::string mName;
			VertexFormat mFormat = VertexFormat::Unknown;
			bool mIsInstanced = false;
			uint8_t mSlot = 0;
		};

		VertexLayout(const std::initializer_list<Element>& elements);
		const std::vector<Element>& GetElements() const;

		static uint32_t GetVertexFormatSize(VertexFormat format);

	  private:
		std::vector<Element> mElements;
	};
}