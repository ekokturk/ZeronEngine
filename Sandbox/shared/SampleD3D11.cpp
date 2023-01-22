// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <SampleD3D11.h>

#include <GUI/ImGui/ImGuiInstance.h>
#include <Platform/Window.h>
#include <Renderer/Image.h>
#include <Renderer/Scene/Camera.h>
#include <Renderer/Scene/Model.h>

#include <Graphics/Buffer.h>
#include <Graphics/CommandBuffer.h>
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsContext.h>
#include <Graphics/Pipeline.h>
#include <Graphics/PipelineBinding.h>
#include <Graphics/Sampler.h>
#include <Graphics/Shader.h>
#include <Graphics/Texture.h>
#include <Graphics/VertexLayout.h>

using namespace Zeron;

namespace SampleD3D11
{
	struct VertexShaderCBData
	{
		Mat4 mViewModelProjectionMatrix;
		Mat4 mWorldMatrix;
	};

	struct PixelShaderCBData
	{
		//float mAlpha = 1.f;
		Vec3 mAmbientLightColor = Vec3::ONE;
		float mAmbientLightStrength = 1.f;

		Vec3 mDynamicLightColor = Vec3::ONE;
		float mDynamicLightStrength = 1.f;
		Vec3 mDynamicLightPosition;
		float mDynamicLightAttenuationA = 1.f;
		float mDynamicLightAttenuationB = 0.1f;
		float mDynamicLightAttenuationC = 0.1f;
	} pixelCB;

	struct SampleContext {
		bool mIsRunning = true;

		Window* mWindow;
		Graphics* mGraphics;

		std::unique_ptr<GraphicsContext> mGraphicsContext;
		std::unique_ptr<ImGuiInstance> mImGui;

		std::unique_ptr<Model> mModel;
		std::unique_ptr<Image> mImage;

		std::unique_ptr<Sampler> mSampler;
		std::unique_ptr<Buffer> mInstanceBuffer;
		std::unique_ptr<Buffer> mPerObjectBuffer;
		std::unique_ptr<Buffer> mLightBuffer;
		std::unique_ptr<ShaderProgram> mShader;
		std::unique_ptr<Texture> mTexture;
		std::unique_ptr<Pipeline> mPipeline;
		std::unique_ptr<PipelineBinding> mPipelineBinding;

		Camera mCamera;
	};

	float cameraSensitivity = 10.f;
	bool isCameraLookAtEnabled = false;
	int instanceM = 10;
	int instanceN = 10;

	SampleInstance::SampleInstance(Zeron::Graphics* graphics, Zeron::Window* window)
		: mCtx(std::make_unique<SampleContext>())
	{
		mCtx->mWindow = window;
		mCtx->mGraphics = graphics;
		auto* gfx = mCtx->mGraphics;

		mCtx->mImGui = std::make_unique<ImGuiInstance>();
		mCtx->mGraphicsContext = gfx->CreateGraphicsContext();
		mCtx->mGraphicsContext->Init(window->GetSystemHandle(), window->GetSize());
		mCtx->mImGui->Init(*gfx, *mCtx->mGraphicsContext);
		mCtx->mCamera.SetPosition({ 0.f, 200, -400.f });
		mCtx->mCamera.SetFieldOfView(60.f);

		std::vector<VertexInstance> instanceData;
		for (int i = 0; i < instanceM; ++i) {
			for (int j = 0; j < instanceN; ++j) {
				instanceData.emplace_back(VertexInstance{ {i * 10.f, j * 10.f, 0 } });
			}
		}
		mCtx->mInstanceBuffer = gfx->CreateVertexBuffer<VertexInstance>(instanceData);

		VertexShaderCBData vertexCB;
		mCtx->mSampler = gfx->CreateSampler();
		mCtx->mPerObjectBuffer = gfx->CreateUniformBuffer<VertexShaderCBData>(vertexCB);
		pixelCB.mAmbientLightColor = { 1.f, 1.f, 1.f };
		mCtx->mLightBuffer = gfx->CreateUniformBuffer<PixelShaderCBData>(pixelCB);
		mCtx->mShader = gfx->CreateShaderProgram("Standard", "Resources/Shaders",
			{
				{"POSITION", VertexFormat::Float3},
				{"TEXTURE_COORD", VertexFormat::Float2},
				{"NORMAL", VertexFormat::Float3},
				{"INSTANCE_POS", VertexFormat::Float3, true, 1},
			},
			{
				{ PipelineResourceType::UniformBuffer, ShaderType::Vertex, 0 },
				{ PipelineResourceType::Texture, ShaderType::Fragment, 1 },
				{ PipelineResourceType::Texture, ShaderType::Fragment, 2 },
				{ PipelineResourceType::Sampler, ShaderType::Fragment, 3 },
				{ PipelineResourceType::UniformBuffer, ShaderType::Fragment, 4 },
			}
			);

		mCtx->mImage = std::make_unique<Image>();
		mCtx->mImage->Load("Resources/Textures/TestHumanoid_CLR.png");
		mCtx->mTexture = gfx->CreateTexture(TextureType::Diffuse, mCtx->mImage->GetColorData().data(), mCtx->mImage->GetWidth(), mCtx->mImage->GetHeight());

		mCtx->mModel = std::make_unique<Model>(*gfx, "Resources/Models/TestHumanoid_Model.fbx", nullptr);
		mCtx->mPipeline = gfx->CreatePipeline(
			mCtx->mShader.get(),
			nullptr,
			MSAALevel::x8,
			PrimitiveTopology::TriangleList,
			true,
			FaceCullMode::Back
		);
		mCtx->mPipelineBinding = gfx->CreatePipelineBinding(
			*mCtx->mPipeline,
			{
							UniformBindingHandle{ mCtx->mPerObjectBuffer.get() },
							TextureBindingHandle{ mCtx->mTexture.get() },
							TextureBindingHandle{ mCtx->mTexture.get() },
							SamplerBindingHandle{ mCtx->mSampler.get() },
							UniformBindingHandle{ mCtx->mLightBuffer.get() },
			}
		);
	}

	SampleInstance::~SampleInstance()
	{
	}

	bool SampleInstance::Run()
	{
		if(!mCtx->mIsRunning) {
			return false;
		}

		ImGuiInstance& imgui = *mCtx->mImGui;
		Camera& camera = mCtx->mCamera;
		GraphicsContext* context = mCtx->mGraphicsContext.get();

		imgui.NewFrame();

		while (mCtx->mWindow->HasSystemEvents()) {
			SystemEvent e = mCtx->mWindow->GetNextSystemEvent();
			if (mCtx->mImGui->HandleEvent(e)) {
				continue;
			}
			std::visit(TypeTraits::Visitor{
				[&](const SystemEvent::WindowClosed&) {
					mCtx->mIsRunning = false;
				},
				[&](const SystemEvent::WindowMinimized&) {
				},
				[&](const SystemEvent::WindowRestored&) {
				},
				[&](const SystemEvent::WindowResized& data) {
					mCtx->mGraphicsContext->ResizeSwapChain(Vec2i(data.mWidth, data.mHeight));
				},
				[&](const SystemEvent::KeyDown& data) {
					if (data.mCode == KeyCode::F) {
						mCtx->mWindow->SetFullScreen(!mCtx->mWindow->IsFullScreen());
					}
					if (data.mCode == KeyCode::H) {
						mCtx->mWindow->SetHidden();
					}
					if (data.mCode == KeyCode::G) {
						mCtx->mWindow->SetAspectRatio(16, 9);
					}
					if (data.mCode == KeyCode::N1) {
						mCtx->mWindow->SetFullScreenType(FullScreenType::Borderless);
					}
					if (data.mCode == KeyCode::N2) {
						mCtx->mWindow->SetFullScreenType(FullScreenType::Monitor);
					}

					if (data.mCode == KeyCode::W) {
						mCtx->mCamera.Move(mCtx->mCamera.GetForwardDir() * cameraSensitivity);
					}
					if (data.mCode == KeyCode::S) {
						mCtx->mCamera.Move(-mCtx->mCamera.GetForwardDir() * cameraSensitivity);
					}
					if (data.mCode == KeyCode::A) {
						mCtx->mCamera.Move(-mCtx->mCamera.GetRightDir() * cameraSensitivity);
					}
					if (data.mCode == KeyCode::D) {
						mCtx->mCamera.Move(mCtx->mCamera.GetRightDir() * cameraSensitivity);
					}
					if (data.mCode == KeyCode::Q) {
						mCtx->mCamera.Move(mCtx->mCamera.GetUpDir() * cameraSensitivity);
					}
					if (data.mCode == KeyCode::E) {
						mCtx->mCamera.Move(-mCtx->mCamera.GetUpDir() * cameraSensitivity);
					}
					if (data.mCode == KeyCode::Up) {
						mCtx->mCamera.Rotate({ Math::ToRadians(15.f),0,0 });
					}
					if (data.mCode == KeyCode::Down) {
						mCtx->mCamera.Rotate({ Math::ToRadians(-15.f),0,0 });
					}
					if (data.mCode == KeyCode::Left) {
						mCtx->mCamera.Rotate({ 0,Math::ToRadians(-15.f),0 });
					}
					if (data.mCode == KeyCode::Right) {
						mCtx->mCamera.Rotate({ 0,Math::ToRadians(15.f),0 });
					}
					if (data.mCode == KeyCode::RightShift) {
						mCtx->mCamera.Rotate({ 0,0,Math::ToRadians(-15.f) });
					}
					if (data.mCode == KeyCode::RightControl) {
						mCtx->mCamera.Rotate({ 0,0,Math::ToRadians(15.f) });
					}
				},
				[&](const SystemEvent::MouseScroll& data) {
					if (data.mOffsetY > 0) {
						mCtx->mCamera.Move(mCtx->mCamera.GetForwardDir() * cameraSensitivity);
					}
					 else {
					  mCtx->mCamera.Move(-mCtx->mCamera.GetForwardDir() * cameraSensitivity);
					}
				},
				[&](const SystemEvent::MouseMoved&) {},

				[](const auto&) { return; },
			}, e.GetData());
		}

		ImGui::Begin("Debug Window");

		ImGui::SliderFloat3("Ambient Light Color", reinterpret_cast<float*>(&pixelCB.mAmbientLightColor), 0.f, 1.f);
		ImGui::SliderFloat("Ambient Light Strength", &pixelCB.mAmbientLightStrength, 0.f, 1.f);
		ImGui::Separator();
		ImGui::DragFloat3("Dynamic Light Position", reinterpret_cast<float*>(&pixelCB.mDynamicLightPosition), 3.f);
		ImGui::SliderFloat("Dynamic Light  Strength", &pixelCB.mDynamicLightStrength, 0.f, 100.f);
		ImGui::SliderFloat("Attenuation A", &pixelCB.mDynamicLightAttenuationA, 0.f, 10.f);
		ImGui::SliderFloat("Attenuation B", &pixelCB.mDynamicLightAttenuationB, 0.f, 10.f);
		ImGui::SliderFloat("Attenuation C", &pixelCB.mDynamicLightAttenuationC, 0.f, 10.f);
		ImGui::Separator();
		float rotation[] = { camera.GetRotation().X, camera.GetRotation().Y, camera.GetRotation().Z };
		if (ImGui::SliderFloat3("Rotation", rotation, -Math::PI<float>(), Math::PI<float>())) {
			camera.SetRotation({ rotation[0], rotation[1], rotation[2] });
		}
		float position[] = { camera.GetPosition().X, camera.GetPosition().Y, camera.GetPosition().Z };
		if (ImGui::DragFloat3("Position", position, -20, 20)) {
			camera.SetPosition({ position[0], position[1], position[2] });
		}
		ImGui::Checkbox("Camera Look At", &isCameraLookAtEnabled);
		bool projPers = camera.GetProjectionType() == Camera::ProjectionType::Perspective;
		if (ImGui::Checkbox("Is Perspective", &projPers)) {
			camera.SetProjectionType(projPers ? Camera::ProjectionType::Perspective : Camera::ProjectionType::Orthographic);
		}
		ImGui::Text("Test");
		ImGui::Separator();
		ImGui::End();

		imgui.Update(*mCtx->mGraphics);

		const Vec2i& viewportSize = context->GetSwapChainSize();

		camera.SetViewSize({ static_cast<float>(viewportSize.X), static_cast<float>(viewportSize.Y) });
		if (isCameraLookAtEnabled) {
			camera.LookAt({ 0,100,0 });
		}

		CommandBuffer& cmd = context->BeginCommands();
		{
			cmd.Clear(Color::DarkRed);
			cmd.SetViewport(viewportSize);
			cmd.SetScissor(viewportSize);

			context->BeginSwapChainRenderPass(cmd);
			cmd.SetPipeline(*mCtx->mPipeline);
			cmd.UpdateBuffer(*mCtx->mLightBuffer, &pixelCB, sizeof(pixelCB));

			for (int i = 0; i < mCtx->mModel->GetMeshes().size(); ++i) {
				auto& mesh = *mCtx->mModel->GetMeshes()[i];
				Mat4 buffer[2];
				buffer[0] = camera.GetProjectionMatrix() * camera.GetViewMatrix() * mesh.GetTransform();
				buffer[1] = mesh.GetTransform();
				cmd.UpdateBuffer(*mCtx->mPerObjectBuffer, &buffer, sizeof(buffer));
				cmd.SetPipelineBinding(*mCtx->mPipelineBinding, 0);
				Buffer* vertexBuff[2] = { mesh.GetVertexBuffer(), mCtx->mInstanceBuffer.get() };
				cmd.SetVertexBuffers(vertexBuff, 2);
				cmd.SetIndexBuffer(*mesh.GetIndexBuffer());
				cmd.DrawInstancedIndexed(mesh.GetIndexBuffer()->GetCount(), instanceM* instanceN);
			}
			imgui.Draw(cmd);
			context->EndSwapChainRenderPass(cmd);
		}
		context->EndCommands();
		context->Submit(cmd);
		context->Present();

		return mCtx->mIsRunning;
	}
}
