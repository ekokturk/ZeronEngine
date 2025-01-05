// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>

namespace Zeron::Gfx
{
	class PushConstant {
	  public:
		struct Element {
			ShaderType mShaderStage;
			uint32_t mTypedBinding = 0;
			uint32_t mStride = 0;
		};

		PushConstant(const std::initializer_list<Element>& elements)
			: mPushConstants(elements)
		{}

		const std::vector<Element>& GetElements() const { return mPushConstants; }

	  private:
		std::vector<Element> mPushConstants;
	};

}