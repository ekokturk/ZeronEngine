// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	enum class VertexFormat {
		Unknown,
		Float2,
		Float3,
	};

	class VertexLayout {
	public:
		
		struct Element {
			std::string mName;
			VertexFormat mFormat = VertexFormat::Unknown;
			bool mIsNormalized = false;
			bool mIsInstanced = false;
		};

		VertexLayout(const std::initializer_list<Element>& elements);
		const std::vector<Element>& GetElements() const;
		void ForEachElement(std::function<void(const VertexLayout::Element&)> callback);

		static uint32_t GetVertexFormatSize(VertexFormat format);
	
	private:
		std::vector<Element> mElements;
	};
}


