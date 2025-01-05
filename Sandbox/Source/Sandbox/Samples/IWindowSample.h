// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Sandbox/Samples/IWindowSampleContext.h>

namespace Zeron
{
	namespace Gfx
	{
		class CommandBuffer;
	}
}

namespace Sandbox
{
	class IWindowSample {
	  public:
		virtual ~IWindowSample() = default;
		virtual void Init() = 0;
		virtual void Update() {}
		virtual void Tick() {}
		virtual void OnFrameBegin(Zeron::Gfx::CommandBuffer&) {}
		virtual void OnFrameEnd(Zeron::Gfx::CommandBuffer&) {}
		virtual void Draw(Zeron::Gfx::CommandBuffer&) {}

		virtual void OnGui() {}
		virtual void OnWindowResized(const Zeron::Vec2i& size) {}

		bool HasGui() const { return mHasGui; }
		void SetShowGui(bool visible) { mShowingGui = visible; }
		bool IsShowingGui() const { return mShowingGui; }

		bool IsInit() const { return mIsInit; }
		bool IsRunning() const { return mIsRunning; }

	  protected:
		IWindowSample(IWindowSampleContext& ctx)
			: mCtx(ctx)
		{}

		IWindowSampleContext& mCtx;
		bool mIsInit = false;
		bool mIsRunning = false;
		bool mShowingGui = false;
		bool mHasGui = true;
	};

}