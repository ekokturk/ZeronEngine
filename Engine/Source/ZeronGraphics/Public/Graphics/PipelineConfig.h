// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>

namespace Zeron::Gfx
{
	struct PipelineConfig {
		MSAALevel mSamplingLevel = MSAALevel::Disabled;
		PrimitiveTopology mTopology = PrimitiveTopology::TriangleList;
		bool mSolidFill = true;
		FaceCullMode mCullMode = FaceCullMode::Back;
		DepthMode mDepthMode = DepthMode::Default;
	};
}