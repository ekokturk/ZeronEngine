// Copyright (C) Eser Kokturk. All Rights Reserved.

#include "VertexLayout.h"

namespace Zeron
{
	VertexLayout::VertexLayout(const std::initializer_list<Element>& elements)
		: mElements(elements)
	{
	}

	const std::vector<VertexLayout::Element>& VertexLayout::GetElements() const
	{
		return mElements;
	}

	void VertexLayout::ForEachElement(std::function<void(const VertexLayout::Element&)> callback)
	{
	}

	uint32_t VertexLayout::GetVertexFormatSize(VertexFormat format)
	{
		switch (format) {
			case VertexFormat::Float2: return sizeof(float) * 2;
			case VertexFormat::Float3: return sizeof(float) * 3;
			case VertexFormat::Unknown:;
			default: return 0;
		}
	}
}
