// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

// Shared includes for Sandbox App

namespace Sandbox
{
	class ISampleInstance {
	  public:
		virtual ~ISampleInstance() = default;
		virtual bool Run() = 0;
	};

	class SampleRunner {
	  public:
		enum class RunCondition
		{
			AllSuccess,
			SingleSuccess
		};

		bool RunAll(RunCondition condition = RunCondition::AllSuccess)
		{
			size_t successCount = mSamples.size();
			for (auto& sample : mSamples) {
				if (!sample->Run()) {
					--successCount;
				}
			}
			switch (condition) {
				case RunCondition::AllSuccess: return successCount == mSamples.size();
				case RunCondition::SingleSuccess: return successCount > 0;
				default: return false;
			}
		}

		template <typename T, typename... Args>
		void AddSample(Args... args)
		{
			mSamples.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		}

	  private:
		std::vector<std::unique_ptr<ISampleInstance>> mSamples;
	};

}