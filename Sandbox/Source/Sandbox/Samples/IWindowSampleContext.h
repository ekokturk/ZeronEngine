// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	namespace Render
	{
		class Camera;
	}
	namespace Gfx
	{
		class ShaderProgram;
		class GraphicsContext;
		class Graphics;
	}
	class Window;
}

namespace Sandbox
{
	class IWindowSampleContext {
	  public:
		virtual ~IWindowSampleContext() = default;
		virtual Zeron::Gfx::Graphics& GetGraphics() = 0;
		virtual Zeron::Gfx::GraphicsContext& GetGraphicsContext() = 0;
		virtual Zeron::Render::Camera& GetCamera() = 0;
		virtual Zeron::Window& GetWindow() = 0;

		virtual std::shared_ptr<Zeron::Gfx::ShaderProgram> LoadShader(const std::string& path) = 0;
	};

}