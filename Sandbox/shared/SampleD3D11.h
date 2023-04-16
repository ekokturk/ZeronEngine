// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Sandbox.h>

namespace Zeron
{
	class Graphics;
	class Window;
}

namespace SampleD3D11
{
	struct SampleContext;

	class SampleInstance : public Sandbox::ISampleInstance {
	  public:
		SampleInstance(Zeron::Graphics* graphics, Zeron::Window* window);
		~SampleInstance() override;
		bool Run() override;

	  private:
		std::unique_ptr<SampleContext> mCtx;
	};
}