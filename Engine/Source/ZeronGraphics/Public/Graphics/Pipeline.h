// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/PipelineConfig.h>

namespace Zeron::Gfx
{
	class Pipeline {
	  public:
		virtual ~Pipeline() = default;

		bool IsCompute() const { return mIsCompute; }
		const PipelineConfig* GetConfig() const { return mConfig.get(); }

	  protected:
		Pipeline()
			: mIsCompute(true)
		{}

		Pipeline(PipelineConfig&& config)
			: mIsCompute(false)
			, mConfig(std::make_unique<PipelineConfig>(std::move(config)))
		{}

	  private:
		bool mIsCompute;
		std::unique_ptr<PipelineConfig> mConfig;
	};
}