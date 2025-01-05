// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Sandbox/App/ISandboxInstance.h>

namespace Sandbox
{
	class SampleInstanceRunner {
	  public:
		SampleInstanceRunner() {}
		~SampleInstanceRunner() { mSamples.clear(); }

		enum class RunCondition
		{
			AllSuccess,
			SingleSuccess
		};

		void ForEach(std::function<void(ISandboxInstance&)> callback)
		{
			for (auto& sample : mSamples) {
				callback(*sample);
			}
		}

		template <typename T, typename... Args>
		void AddSample(Args... args)
		{
			mSamples.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		}

		void Destroy() { mSamples.clear(); }

	  private:
		std::vector<std::unique_ptr<ISandboxInstance>> mSamples;
	};

}