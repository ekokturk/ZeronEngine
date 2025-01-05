// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Sandbox/App/SandboxInstanceRunner.h>

namespace Zeron
{
	namespace Gfx
	{
		class Graphics;
		enum class GraphicsType;
	}
	class Platform;
}

namespace Sandbox
{
	class Application {
	  public:
		Application(Zeron::Platform& platform);
		~Application();
		void Run();

	  protected:
		void _init();
		void _update();
		void _tick();

		Zeron::Platform& mPlatform;
		std::unordered_map<Zeron::Gfx::GraphicsType, std::unique_ptr<Zeron::Gfx::Graphics>> mGraphics;
		Sandbox::SampleInstanceRunner mSampleRunner;
	};
}