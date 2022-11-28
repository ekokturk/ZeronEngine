// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <SampleD3D11.h>

#include <Core/Math/Mat4.h>
#include <Core/Math/Vec3.h>
#include <Core/Types/Color.h>
#include <GUI/ImGui/ImGuiInstance.h>
#include <Renderer/Image.h>
#include <Renderer/Scene/Camera.h>
#include <Renderer/Scene/Model.h>
#include <Window/Window.h>

#include <Graphics/Buffer.h>
#include <Graphics/CommandBuffer.h>
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

namespace {
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

	struct WindowContext {
		std::unique_ptr<Graphics> mGraphics;
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
		bool isInit = false;
		bool isRunning = true;
	} ctx;

	float cameraSensitivity = 10.f;
	bool isCameraLookAtEnabled = false;
	int instanceM = 10;
	int instanceN = 10;
}

bool SampleD3D11::Run(Window* window)
{
	if (!ctx.isInit) {
		window->Init();
		ctx.mGraphics = Graphics::CreateGraphics(GraphicsType::Direct3D11);
		if (!ctx.mGraphics->Init()) {
			return false;
		}
		auto* gfx = ctx.mGraphics.get();

		ctx.mImGui = std::make_unique<ImGuiInstance>();
		ctx.mGraphicsContext = gfx->CreateGraphicsContext();
		ctx.mGraphicsContext->Init(window->GetSystemHandle(), window->GetSize());
		ctx.mImGui->Init(*gfx, *ctx.mGraphicsContext);
		ctx.mCamera.SetPosition({ 0.f, 200, -400.f });
		ctx.mCamera.SetFieldOfView(60.f);

		std::vector<VertexInstance> instanceData;
		for (int i = 0; i < instanceM; ++i) {
			for (int j = 0; j < instanceN; ++j) {
				instanceData.emplace_back(VertexInstance{ {i * 10.f, j * 10.f, 0 } });
			}
		}
		ctx.mInstanceBuffer = gfx->CreateVertexBuffer<VertexInstance>(instanceData);

		VertexShaderCBData vertexCB;
		ctx.mSampler = gfx->CreateSampler();
		ctx.mPerObjectBuffer = gfx->CreateUniformBuffer<VertexShaderCBData>(vertexCB);
		pixelCB.mAmbientLightColor = { 1.f, 1.f, 1.f };
		ctx.mLightBuffer = gfx->CreateUniformBuffer<PixelShaderCBData>(pixelCB);
		ctx.mShader  = gfx->CreateShaderProgram("Standard", "Resources/Shaders",
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

		ctx.mImage = std::make_unique<Image>();
		ctx.mImage->Load("Resources/Textures/TestHumanoid_CLR.png");
		ctx.mTexture = gfx->CreateTexture(TextureType::Diffuse, ctx.mImage->GetColorData().data(), ctx.mImage->GetWidth(), ctx.mImage->GetHeight());

		ctx.mModel = std::make_unique<Model>(*gfx, "Resources/Models/TestHumanoid_Model.fbx", nullptr);
		ctx.mPipeline = gfx->CreatePipeline(
			ctx.mShader.get(),
			nullptr,
			MSAALevel::x8,
			PrimitiveTopology::TriangleList,
			true,
			FaceCullMode::Back
		);
		ctx.mPipelineBinding = gfx->CreatePipelineBinding(
			*ctx.mPipeline,
			{
							UniformBindingHandle{ ctx.mPerObjectBuffer.get() },
							TextureBindingHandle{ ctx.mTexture.get() },
							TextureBindingHandle{ ctx.mTexture.get() },
							SamplerBindingHandle{ ctx.mSampler.get() },
							UniformBindingHandle{ ctx.mLightBuffer.get() },
			}
		);
		ctx.isInit = true;
	}

	ImGuiInstance& imgui = *ctx.mImGui;
	Camera& camera = ctx.mCamera;
	GraphicsContext* context = ctx.mGraphicsContext.get();

	window->Update();
	imgui.NewFrame();

	while (auto e = window->GetNextEvent()) {
		if (imgui.HandleEvent(*e)) {
			continue;
		}

		if (e->GetID() == WindowEventID::WindowClosed) {
			return false;
		}
		if (e->GetID() == WindowEventID::KeyDown) {
			auto& procEvnt = static_cast<WindowEvent_KeyDown&>(*e);
			std::cout << window->GetName() << " " << procEvnt.GetEventName() << " " << procEvnt.mCode.ToString() << std::endl;
			if (procEvnt.mCode == KeyCode::F) {
				window->SetFullScreen(!window->IsFullScreen());
			}
			if (procEvnt.mCode == KeyCode::H) {
				window->SetHidden();
			}
			if (procEvnt.mCode == KeyCode::G) {
				window->SetAspectRatio(16, 9);
			}
			if (procEvnt.mCode == KeyCode::N1) {
				window->SetFullScreenType(FullScreenType::Borderless);
			}
			if (procEvnt.mCode == KeyCode::N2) {
				window->SetFullScreenType(FullScreenType::Monitor);
			}

			if (procEvnt.mCode == KeyCode::W) {
				camera.Move(camera.GetForwardDir() * -cameraSensitivity);
			}
			if (procEvnt.mCode == KeyCode::S) {
				camera.Move(camera.GetForwardDir() * cameraSensitivity);
			}
			if (procEvnt.mCode == KeyCode::A) {
				camera.Move(camera.GetRightDir() * -cameraSensitivity);
			}
			if (procEvnt.mCode == KeyCode::D) {
				camera.Move(camera.GetRightDir() * cameraSensitivity);
			}
			if (procEvnt.mCode == KeyCode::Q) {
				camera.Move(camera.GetUpDir() * -cameraSensitivity);
			}
			if (procEvnt.mCode == KeyCode::E) {
				camera.Move(camera.GetUpDir() * cameraSensitivity);
			}

			if (procEvnt.mCode == KeyCode::Up) {
				camera.Rotate({ Math::ToRadians(15.f),0,0 });
			}
			if (procEvnt.mCode == KeyCode::Down) {
				camera.Rotate({ Math::ToRadians(-15.f),0,0 });
			}
			if (procEvnt.mCode == KeyCode::Left) {
				camera.Rotate({ 0,Math::ToRadians(-15.f),0 });
			}
			if (procEvnt.mCode == KeyCode::Right) {
				camera.Rotate({ 0,Math::ToRadians(15.f),0 });
			}
			if (procEvnt.mCode == KeyCode::RightShift) {
				camera.Rotate({ 0,0,Math::ToRadians(-15.f) });
			}
			if (procEvnt.mCode == KeyCode::RightControl) {
				camera.Rotate({ 0,0,Math::ToRadians(15.f) });
			}
		}
		if (e->GetID() == WindowEventID::KeyUp) {
			auto& procEvnt = static_cast<WindowEvent_KeyUp&>(*e);
			std::cout << window->GetName() << " " << procEvnt.GetEventName() << " " << procEvnt.mCode.ToString() << std::endl;
		}
		if (e->GetID() == WindowEventID::MouseButtonDown) {
			auto& procEvnt = static_cast<WindowEvent_MouseDown&>(*e);
			std::cout << window->GetName() << procEvnt.GetEventName() << " " << procEvnt.mCode.ToString() << std::endl;
		}
		else if (e->GetID() == WindowEventID::TextChar) {
			auto& procEvnt = static_cast<WindowEvent_TextChar&>(*e);
			ZE_LOG("{}", procEvnt.mCharacter);
			//std::cout << procEvnt.GetEventName() << " " << procEvnt.mCharacter << std::endl;
		}
		else if (e->GetID() == WindowEventID::MouseEnter) {
			auto& procEvnt = static_cast<WindowEvent_MouseEnter&>(*e);
			std::cout << procEvnt.GetEventName() << std::endl;
		}
		else if (e->GetID() == WindowEventID::MouseExit) {
			auto& procEvnt = static_cast<WindowEvent_MouseExit&>(*e);
			std::cout << procEvnt.GetEventName() << std::endl;
		}
		else if (e->GetID() == WindowEventID::WindowResized) {
			auto& procEvnt = static_cast<WindowEvent_WindowResized&>(*e);
			context->ResizeSwapChain({ procEvnt.mWidth, procEvnt.mHeight });
		}
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

	imgui.Update(*ctx.mGraphics);

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
		cmd.SetPipeline(*ctx.mPipeline);
		cmd.UpdateBuffer(*ctx.mLightBuffer, &pixelCB, sizeof(pixelCB));

		for (int i = 0; i < ctx.mModel->GetMeshes().size(); ++i) {
			auto& mesh = *ctx.mModel->GetMeshes()[i];
			Mat4 buffer[2];
			buffer[0] = camera.GetProjectionMatrix() * camera.GetViewMatrix() * mesh.GetTransform();
			buffer[1] = mesh.GetTransform();
			cmd.UpdateBuffer(*ctx.mPerObjectBuffer, &buffer, sizeof(buffer));
			cmd.SetPipelineBinding(*ctx.mPipelineBinding, 0);
			Buffer* vertexBuff[2] = { mesh.GetVertexBuffer(), ctx.mInstanceBuffer.get() };
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

	return true;
}
