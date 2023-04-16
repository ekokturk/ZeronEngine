// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>

namespace Zeron
{
	class ResourceLayout {
	  public:
		struct Element {
			PipelineResourceType mType;
			ShaderType mShaderStage;
			uint32_t mBinding;
			uint32_t mSet = 0;
		};

		ResourceLayout(const std::initializer_list<Element>& elements)
			: mResourceList(elements)
		{}

		const std::vector<Element>& GetResources() const { return mResourceList; }

	  private:
		std::vector<Element> mResourceList;
	};

}