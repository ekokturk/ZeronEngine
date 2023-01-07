// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Sandbox.h>

namespace Zeron {
	class Graphics;
	class Window;
}

namespace SampleVulkan
{
	struct SampleContext;

	class SampleInstance : public Sandbox::ISampleInstance {
	public:
		SampleInstance(Zeron::Graphics* graphics, Zeron::Window* window);
		~SampleInstance();
		bool Run() override;
	private:
		std::unique_ptr<SampleContext> mCtx;
	};
}


