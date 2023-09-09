// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

// Shared includes for Sandbox App

namespace Sandbox
{
	class ISampleInstance {
	  public:
		virtual ~ISampleInstance() = default;
		virtual void Init() = 0;
		virtual void Update() {}
		virtual void FixedUpdate() {}

		bool IsInit() const { return mIsInit; }
		bool IsRunning() const { return mIsRunning; }

	  protected:
		bool mIsInit = false;
		bool mIsRunning = false;
	};

	class SampleRunner {
	  public:
		SampleRunner() {}
		~SampleRunner() { mSamples.clear(); }

		enum class RunCondition
		{
			AllSuccess,
			SingleSuccess
		};

		void ForEach(std::function<void(ISampleInstance&)> callback)
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
		std::vector<std::unique_ptr<ISampleInstance>> mSamples;
	};

}