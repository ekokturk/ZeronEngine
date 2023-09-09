// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Sandbox.h>

namespace Zeron
{
	namespace Gfx
	{
		class Graphics;
	}
	class Window;
}

namespace SampleGraphics
{
	struct SampleContext;

	class SampleInstance : public Sandbox::ISampleInstance {
	  public:
		SampleInstance(Zeron::Gfx::Graphics* graphics, Zeron::Window* window);
		~SampleInstance() override;
		void Init() override;
		void Update() override;

	  private:
		std::unique_ptr<SampleContext> mCtx;
	};
}