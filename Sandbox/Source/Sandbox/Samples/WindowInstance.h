// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Render/Camera/Camera.h>
#include <Sandbox/App/ISandboxInstance.h>
#include <Sandbox/Samples/IWindowSampleContext.h>

namespace Zeron
{
	class ImGuiInstance;
	namespace Gfx
	{
		class Window;
		class GraphicsContext;
		class Graphics;
	}
	class Window;
}

namespace Sandbox
{
	struct TestSampleContext;
	class IWindowSample;

	class WindowInstance : public ISandboxInstance, public IWindowSampleContext {
	  public:
		WindowInstance(Zeron::Gfx::Graphics* graphics, Zeron::Window* window);
		~WindowInstance() override;

		// ISandboxInstance
		void Init() override;
		void Tick() override;
		void Update() override;

		// IWindowSampleContext
		Zeron::Gfx::Graphics& GetGraphics() override;
		Zeron::Gfx::GraphicsContext& GetGraphicsContext() override;
		Zeron::Render::Camera& GetCamera() override;
		Zeron::Window& GetWindow() override;
		std::shared_ptr<Zeron::Gfx::ShaderProgram> LoadShader(const std::string& path) override;

	  private:
		void _processInput();
		void _onFrameBegin();
		void _OnGui();
		void _drawFrame();

		template <typename T>
		T* _getActiveSample()
		{
			auto sample = mActiveSamples.find(T::NAME);
			return sample != mActiveSamples.end() ? static_cast<T*>(sample->second.get()) : nullptr;
		}
		void _toggleSample(const std::string& name, bool enabled);

		bool mIsSuspended = false;

		Zeron::Window* mWindow;
		Zeron::Gfx::Graphics* mGraphics;

		Zeron::Render::Camera mCamera;
		float mCameraSensitivity = 1.f;
		bool mIsCameraLookAtEnabled = false;
		bool mIsGuiEnabled = true;

		std::unique_ptr<Zeron::ImGuiInstance> mImGui;
		std::unordered_map<std::string, std::function<std::unique_ptr<IWindowSample>()>> mSampleFactory;

		std::map<std::string, std::unique_ptr<IWindowSample>> mActiveSamples;
		std::unique_ptr<TestSampleContext> mTestSampleCtx;

		std::unique_ptr<Zeron::Gfx::GraphicsContext> mGraphicsContext;
	};
}