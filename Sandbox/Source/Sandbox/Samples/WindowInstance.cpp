// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Sandbox/Samples/WindowInstance.h>

#include <Core/FileSystem.h>
#include <Graphics/Buffer.h>
#include <Graphics/CommandBuffer.h>
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsContext.h>
#include <Graphics/Pipeline.h>
#include <Graphics/PipelineBinding.h>
#include <Graphics/ShaderProgram.h>
#include <Graphics/ShaderProgramConfig.h>
#include <GUI/ImGui/ImGuiInstance.h>
#include <Platform/Window.h>
#include <Render/Camera/Camera.h>

#include <Sandbox/Samples/Graphics/InstancedRenderingSample.h>
#include <Sandbox/Samples/Graphics/SkyboxRenderingSample.h>
#include <Sandbox/Samples/Graphics/TextRenderingSample.h>
#include <Sandbox/Samples/Graphics/WireframePhysicsSample.h>
#include <Sandbox/Samples/Networking/WebSocketSample.h>

using namespace Zeron;

namespace Sandbox
{
	struct TestSampleContext {
		// Compute
		WindowInstance& mInstance;
		std::unique_ptr<Gfx::CommandBuffer> mComputeCommandBuffer;
		std::shared_ptr<Gfx::ShaderProgram> mComputeShader;
		std::unique_ptr<Gfx::Pipeline> mComputePipeline;
		std::unique_ptr<Gfx::PipelineBinding> mComputePipelineBinding;
		std::unique_ptr<Gfx::Buffer> mComputeStorageBuffer;


		TestSampleContext(WindowInstance& instance)
			: mInstance(instance)
		{}

		void Init(Gfx::Graphics& graphics)
		{
			mComputeCommandBuffer = graphics.CreateCommandBuffer(1, true);
			mComputeShader = mInstance.LoadShader("LightCulling");
			mComputePipeline = graphics.CreatePipelineCompute(*mComputeShader);
			mComputeStorageBuffer = graphics.CreateStorageBuffer(std::vector<float>(100), true);
			mComputePipelineBinding = graphics.CreatePipelineBinding(
				*mComputePipeline,
				{ {
					Gfx::StorageBufferBindingHandle{ mComputeStorageBuffer.get() },
				} }
			);
		}
	};

	WindowInstance::WindowInstance(Gfx::Graphics* graphics, Zeron::Window* window)
		: mWindow(window)
		, mGraphics(graphics)
		, mTestSampleCtx(std::make_unique<TestSampleContext>(*this))
	{}

	WindowInstance::~WindowInstance()
	{
		mGraphicsContext.reset();
		mActiveSamples.clear();
	};

	void WindowInstance::Init()
	{
		mIsInit = true;
		mIsRunning = true;

		mGraphicsContext = mGraphics->CreateGraphicsContext();
		mGraphicsContext->Init(mWindow->GetSystemHandle(), mWindow->GetSize());
		mImGui = std::make_unique<ImGuiInstance>();
		mImGui->Init(*mGraphics, *mGraphicsContext, LoadShader("ImGui"));

		// Test Sample
		mTestSampleCtx->Init(*mGraphics);

		// Examples
		mSampleFactory.emplace(SkyboxRenderingSample::NAME, [&]() {
			return std::make_unique<SkyboxRenderingSample>(*this);
		});
		mSampleFactory.emplace(TextRenderingSample::NAME, [&]() {
			return std::make_unique<TextRenderingSample>(*this);
		});
		mSampleFactory.emplace(InstancedRenderingSample::NAME, [&]() {
			return std::make_unique<InstancedRenderingSample>(*this);
		});
		mSampleFactory.emplace(WireframePhysicsSample::NAME, [&]() {
			return std::make_unique<WireframePhysicsSample>(*this);
		});
		mSampleFactory.emplace(WebSocketSample::NAME, [&]() {
			return std::make_unique<WebSocketSample>(*this);
		});


		for (auto& [name, fn] : mSampleFactory) {
			_toggleSample(name, true);
		}

		mCamera.SetPosition({ 0.f, 2, -4 });
		mCamera.SetFieldOfView(60.f);
	}
	void WindowInstance::Tick()
	{
		for (auto& [_, sample] : mActiveSamples) {
			sample->Tick();
		}
	}

	void WindowInstance::Update()
	{
		if (!mIsRunning) {
			return;
		}
		_processInput();
		if (!mIsSuspended) {
			for (auto& [_, sample] : mActiveSamples) {
				sample->Update();
			}
			_onFrameBegin();
			_drawFrame();
		}
	}

	Gfx::Graphics& WindowInstance::GetGraphics()
	{
		return *mGraphics;
	}

	Gfx::GraphicsContext& WindowInstance::GetGraphicsContext()
	{
		return *mGraphicsContext;
	}

	Render::Camera& WindowInstance::GetCamera()
	{
		return mCamera;
	}

	Window& WindowInstance::GetWindow()
	{
		return *mWindow;
	}

	std::shared_ptr<Gfx::ShaderProgram> WindowInstance::LoadShader(const std::string& name)
	{
		const std::string infoFile = name + ".zsl.json";
		const Path shaderDir{ "Resources/Shaders", PathType::Assets };
		auto standardMeta = Locator::Get<FileSystem>()->ReadTextFile(shaderDir / infoFile);
		ZE_ASSERT(!standardMeta.HasError(), "Expected shader meta data to be found.");
		std::unordered_map<Gfx::ShaderType, ByteBuffer> shaderData;
		Gfx::ShaderProgramConfig shaderProgramConfig(standardMeta.Value());
		for (Gfx::ShaderType type : shaderProgramConfig.mStages) {
			auto file = Locator::Get<FileSystem>()->ReadBinaryFile(shaderDir / GetGraphics().GetCompiledShaderName(name, type));
			ZE_ASSERT(!file.HasError(), "Expected shader type to be found.");
			shaderData.emplace(type, std::move(file.Value()));
		}

		return GetGraphics().CreateShaderProgram(shaderProgramConfig, std::move(shaderData));
	}

	void WindowInstance::_onFrameBegin()
	{
		if (mIsGuiEnabled) {
			_OnGui();
		}
		for (auto& [_, sample] : mActiveSamples) {
			if (sample->IsShowingGui()) {
				if (mIsGuiEnabled) {
					sample->OnGui();
				}
			}
		}
	}
	void WindowInstance::_OnGui()
	{
		mImGui->NewFrame();

		ImGui::Begin("App", 0, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Close")) {
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
				float position[] = { mCamera.GetPosition().X, mCamera.GetPosition().Y, mCamera.GetPosition().Z };
				if (ImGui::DragFloat3("Position", position, -1, 1)) {
					mCamera.SetPosition({ position[0], position[1], position[2] });
				}
				if (!mIsCameraLookAtEnabled) {
					float rotation[] = { mCamera.GetRotation().X, mCamera.GetRotation().Y, mCamera.GetRotation().Z };
					if (ImGui::SliderFloat3("Rotation", rotation, -Math::PI<float>(), Math::PI<float>())) {
						mCamera.SetRotation({ rotation[0], rotation[1], rotation[2] });
					}
				}
				ImGui::Checkbox("Camera Look At", &mIsCameraLookAtEnabled);
				bool projPers = mCamera.GetProjectionType() == Render::Camera::ProjectionType::Perspective;
				if (ImGui::Checkbox("Is Perspective", &projPers)) {
					mCamera.SetProjectionType(projPers ? Render::Camera::ProjectionType::Perspective : Render::Camera::ProjectionType::Orthographic);
				}
			}

			if (ImGui::CollapsingHeader("Samples", ImGuiTreeNodeFlags_DefaultOpen)) {
				for (auto& [name, _] : mSampleFactory) {
					ImGui::PushID(name.c_str());
					ImGui::BeginGroup();
					bool enabled = mActiveSamples.contains(name);
					if (ImGui::Checkbox(name.c_str(), &enabled)) {
						_toggleSample(name, enabled);
					}
					auto foundSample = mActiveSamples.find(name);
					if (foundSample != mActiveSamples.end()) {
						auto& activeSample = *foundSample->second;
						if (activeSample.HasGui()) {
							ImGui::SameLine();
							bool hasGui = activeSample.IsShowingGui();
							if (ImGui::Checkbox("GUI", &hasGui)) {
								activeSample.SetShowGui(hasGui);
							}
						}
					}
					ImGui::EndGroup();
					ImGui::PopID();
				}
			}
		}
		ImGui::End();
	}

	void WindowInstance::_drawFrame()
	{
		const Vec2i& viewportSize = mGraphicsContext->GetSwapChainSize();

		mCamera.SetViewSize({ static_cast<float>(viewportSize.X), static_cast<float>(viewportSize.Y) });
		if (mIsCameraLookAtEnabled) {
			mCamera.LookAt({ 0, 1, 0 });
		}

		Gfx::CommandBuffer& cmd = mGraphicsContext->BeginCommands();
		{
			cmd.Clear(Color::DarkRed);
			cmd.SetViewport(viewportSize);
			cmd.SetScissor(viewportSize);

			cmd.BeginDebugGroup("Frame Begin");
			{
				for (auto& sample : mActiveSamples) {
					sample.second->OnFrameBegin(cmd);
				}
				{
					cmd.BeginDebugGroup("Compute Sample");
					cmd.SetPipeline(*mTestSampleCtx->mComputePipeline);
					cmd.SetPipelineBinding(*mTestSampleCtx->mComputePipelineBinding);
					cmd.Dispatch(16, 16, 1);
					cmd.EndDebugGroup();
				}
			}
			cmd.EndDebugGroup();

			cmd.BeginDebugGroup("Frame Main Pass");
			mGraphicsContext->BeginSwapChainRenderPass(cmd);
			{
				for (auto& sample : mActiveSamples) {
					sample.second->Draw(cmd);
				}
				if (mIsGuiEnabled) {
					cmd.BeginDebugGroup("ImGui");
					mImGui->Update(*mGraphics);
					mImGui->Draw(cmd);
					cmd.EndDebugGroup();
				}
			}
			mGraphicsContext->EndSwapChainRenderPass(cmd);
			cmd.EndDebugGroup();

			cmd.BeginDebugGroup("Frame End");
			{
				for (auto& sample : mActiveSamples) {
					sample.second->OnFrameEnd(cmd);
				}
			}
			cmd.EndDebugGroup();
		}
		mGraphicsContext->EndCommands();
		mGraphicsContext->Submit(cmd);
		mGraphicsContext->Present();
	}

	void WindowInstance::_toggleSample(const std::string& name, bool enabled)
	{
		auto factory = mSampleFactory.find(name);
		if (factory == mSampleFactory.end()) {
			ZE_LOGW("Sample '{}' is not registered", name);
			return;
		}

		if (enabled && !mActiveSamples.contains(name)) {
			auto sample = factory->second();
			sample->Init();
			mActiveSamples.emplace(name, std::move(sample));
		}
		else {
			mActiveSamples.erase(name);
		}
	}

	void WindowInstance::_processInput()
	{
		while (mWindow->HasSystemEvents()) {
			SystemEvent e = mWindow->GetNextSystemEvent();
			if (mImGui->HandleEvent(e)) {
				continue;
			}
			std::visit(
				TypeTraits::Visitor{
					[&](const SystemEvent::WindowClosed&) {
						mIsRunning = false;
					},
					[&](const SystemEvent::WindowMinimized&) {
						mIsSuspended = true;
					},
					[&](const SystemEvent::WindowRestored&) {
						mIsSuspended = false;
					},
					[&](const SystemEvent::WindowResized& data) {
						Vec2i windowSize(data.mWidth, data.mHeight);
						mGraphicsContext->ResizeSwapChain(windowSize);
						for (auto& sample : mActiveSamples) {
							sample.second->OnWindowResized(windowSize);
						}
					},
					[&](const SystemEvent::KeyDown& data) {
						if (data.mCode == KeyCode::W) {
							mCamera.Move(mCamera.GetForwardDir() * mCameraSensitivity);
						}
						if (data.mCode == KeyCode::S) {
							mCamera.Move(-mCamera.GetForwardDir() * mCameraSensitivity);
						}
						if (data.mCode == KeyCode::A) {
							mCamera.Move(-mCamera.GetRightDir() * mCameraSensitivity);
						}
						if (data.mCode == KeyCode::D) {
							mCamera.Move(mCamera.GetRightDir() * mCameraSensitivity);
						}
						if (data.mCode == KeyCode::Q) {
							mCamera.Move(mCamera.GetUpDir() * mCameraSensitivity);
						}
						if (data.mCode == KeyCode::E) {
							mCamera.Move(-mCamera.GetUpDir() * mCameraSensitivity);
						}
						if (data.mCode == KeyCode::Up) {
							mCamera.Rotate({ Math::ToRadians(15.f), 0, 0 });
						}
						if (data.mCode == KeyCode::Down) {
							mCamera.Rotate({ Math::ToRadians(-15.f), 0, 0 });
						}
						if (data.mCode == KeyCode::Left) {
							mCamera.Rotate({ 0, Math::ToRadians(-15.f), 0 });
						}
						if (data.mCode == KeyCode::Right) {
							mCamera.Rotate({ 0, Math::ToRadians(15.f), 0 });
						}
						if (data.mCode == KeyCode::RightShift) {
							mCamera.Rotate({ 0, 0, Math::ToRadians(-15.f) });
						}
						if (data.mCode == KeyCode::RightControl) {
							mCamera.Rotate({ 0, 0, Math::ToRadians(15.f) });
						}
					},
					[&](const SystemEvent::MouseScroll& data) {
						if (data.mOffsetY > 0) {
							mCamera.Move(mCamera.GetForwardDir() * mCameraSensitivity);
						}
						else {
							mCamera.Move(-mCamera.GetForwardDir() * mCameraSensitivity);
						}
					},
					[&](const SystemEvent::MouseMoved&) {},

					[](const auto&) {
						return;
					},
				},
				e.GetData()
			);
		}
	}
}