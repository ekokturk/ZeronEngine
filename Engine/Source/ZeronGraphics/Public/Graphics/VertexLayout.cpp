﻿// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/VertexLayout.h>

namespace Zeron::Gfx
{
	VertexLayout::VertexLayout(const std::initializer_list<Element>& elements)
		: mElements(elements)
	{}

	const std::vector<VertexLayout::Element>& VertexLayout::GetElements() const
	{
		return mElements;
	}

	uint32_t VertexLayout::GetVertexFormatSize(VertexFormat format)
	{
		switch (format) {
			case VertexFormat::Float2: return sizeof(float) * 2;
			case VertexFormat::Float3: return sizeof(float) * 3;
			case VertexFormat::Float4: return sizeof(float) * 4;
			case VertexFormat::Color: return sizeof(Color);
			case VertexFormat::Unknown:;
			default: return 0;
		}
	}
}