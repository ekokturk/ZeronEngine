// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <SampleVulkan.h>

#include <GUI/ImGui/ImGuiInstance.h>
#include <Renderer/Image.h>
#include <Renderer/Scene/Camera.h>
#include <Renderer/Scene/Model.h>
#include <Window/Window.h>

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
	};

	struct MeshResource {
		std::unique_ptr<Buffer> mUniformBuffer;
		std::unique_ptr<Buffer> mLightBuffer;
		std::unique_ptr<PipelineBinding> mBinding;
	};

	struct WindowContext {
		std::unique_ptr<Graphics> mGraphics;
		std::unique_ptr<GraphicsContext> mGraphicsContext;
		std::unique_ptr<ImGuiInstance> mImGui;

		std::unique_ptr<Sampler> mSampler;
		std::unique_ptr<Buffer> mInstanceBuffer;
		std::unique_ptr<ShaderProgram> mShader;
		std::unique_ptr<Texture> mTexture;
		std::unique_ptr<Pipeline> mPipeline;

		std::vector<MeshResource> mMeshResources;

		std::unique_ptr<Model> mModel;
		std::unique_ptr<Image> mImage;

		Camera mCamera;
		bool isInit = false;

		// Compute
		std::unique_ptr<CommandBuffer> mComputeCommandBuffer;
		std::unique_ptr<ShaderProgram> mComputeShader;
		std::unique_ptr<Pipeline> mComputePipeline;

	} ctx;

	float cameraSensitivity = 10.f;
	int instanceM = 10;
	int instanceN = 10;
	bool isSuspended = false;

}

bool SampleVulkan::Run(Zeron::Window* window)
{
	if(!ctx.isInit) {
		window->Init();
		ctx.mGraphics = Graphics::CreateGraphics(GraphicsType::Vulkan);
		ctx.mGraphics->Init();
		auto* gfx = ctx.mGraphics.get();
		ctx.mGraphicsContext = gfx->CreateGraphicsContext();
		ctx.mGraphicsContext->Init(window->GetSystemHandle(), window->GetSize());
		ctx.mImGui = std::make_unique<ImGuiInstance>();
		ctx.mImGui->Init(*gfx, *ctx.mGraphicsContext);

		std::vector<VertexInstance> instanceData;
		for (int i = 0; i < instanceM; ++i) {
			for (int j = 0; j < instanceN; ++j) {
				instanceData.emplace_back(VertexInstance{ {i * 10.f, j * 10.f, 0 } });
			}
		}

		ctx.mInstanceBuffer = gfx->CreateVertexBuffer<VertexInstance>(instanceData);

		// Compute
		ctx.mComputeCommandBuffer = gfx->CreateCommandBuffer(1, true);
		ctx.mComputeShader = gfx->CreateShaderProgram("ComputeTest", "Resources/Shaders", {}, {});
		ctx.mComputePipeline = gfx->CreatePipeline(ctx.mComputeShader.get());


		ctx.mShader = gfx->CreateShaderProgram("Standard", "Resources/Shaders", {
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

		ctx.mPipeline = gfx->CreatePipeline(
			ctx.mShader.get(),
			ctx.mGraphicsContext->GetSwapChainRenderPass(),
			gfx->GetMultiSamplingLevel(),
			PrimitiveTopology::TriangleList,
			true,
			FaceCullMode::Back
		);

		ctx.mImage = std::make_unique<Image>();
		ctx.mImage->Load("Resources/Textures/TestHumanoid_CLR.png");
		ctx.mTexture = gfx->CreateTexture(TextureType::Diffuse, ctx.mImage->GetColorData().data(), ctx.mImage->GetWidth(), ctx.mImage->GetHeight());

		ctx.mSampler = gfx->CreateSampler();

		ctx.mModel = std::make_unique<Model>(*gfx, "Resources/Models/TestHumanoid_Model.fbx", nullptr);
		for (auto& mesh : ctx.mModel->GetMeshes()) {
			MeshResource res;
			res.mUniformBuffer = gfx->CreateUniformBuffer<VertexShaderCBData>(VertexShaderCBData{});
			res.mLightBuffer = gfx->CreateUniformBuffer<PixelShaderCBData>(PixelShaderCBData{});
			res.mBinding = gfx->CreatePipelineBinding(*ctx.mPipeline, std::vector<BindingHandle>{
				UniformBindingHandle{ res.mUniformBuffer.get() },
					TextureBindingHandle{ ctx.mTexture.get() },
					TextureBindingHandle{ ctx.mTexture.get() },
					SamplerBindingHandle{ ctx.mSampler.get() },
					UniformBindingHandle{ res.mLightBuffer.get() },
			});
			ctx.mMeshResources.push_back(std::move(res));
		}

		ctx.isInit = true;

		ctx.mCamera.SetPosition({ 0.f, 200.f, -400.f });
		ctx.mCamera.SetFieldOfView(60.f);
	}
		
	window->Update();
	ctx.mImGui->NewFrame();

	while (auto e = window->GetNextEvent()) {
		if (ctx.mImGui->HandleEvent(*e)) {
			continue;
		}

		if (e->GetID() == WindowEventID::WindowClosed) {
			return false;
		}
		else if (e->GetID() == WindowEventID::WindowResized) {
			auto& procEvnt = static_cast<WindowEvent_WindowResized&>(*e);
			ctx.mGraphicsContext->ResizeSwapChain(Vec2i(procEvnt.mWidth, procEvnt.mHeight));
		}
		else if (e->GetID() == WindowEventID::WindowMinimized) {
			isSuspended = true;
		}
		else if (e->GetID() == WindowEventID::WindowRestored) {
			isSuspended = false;
		}
		else if (e->GetID() == WindowEventID::MouseScroll) {
			auto& procEvnt = static_cast<WindowEvent_MouseScrolled&>(*e);
			if(procEvnt.mOffsetY > 0) {
				ctx.mCamera.Move(ctx.mCamera.GetForwardDir() * cameraSensitivity);
			} else {
				ctx.mCamera.Move(-ctx.mCamera.GetForwardDir() * cameraSensitivity);
			}

		}
		if (e->GetID() == WindowEventID::KeyDown) {
			auto& procEvnt = static_cast<WindowEvent_KeyDown&>(*e);
			if (procEvnt.mCode == KeyCode::W) {
				ctx.mCamera.Move(ctx.mCamera.GetForwardDir() * cameraSensitivity);
			}
			if (procEvnt.mCode == KeyCode::S) {
				ctx.mCamera.Move(-ctx.mCamera.GetForwardDir() * cameraSensitivity);
			}
			if (procEvnt.mCode == KeyCode::A) {
				ctx.mCamera.Move(-ctx.mCamera.GetRightDir() * cameraSensitivity);
			}
			if (procEvnt.mCode == KeyCode::D) {
				ctx.mCamera.Move(ctx.mCamera.GetRightDir() * cameraSensitivity);
			}
			if (procEvnt.mCode == KeyCode::Q) {
				ctx.mCamera.Move(ctx.mCamera.GetUpDir() * cameraSensitivity);
			}
			if (procEvnt.mCode == KeyCode::E) {
				ctx.mCamera.Move(-ctx.mCamera.GetUpDir() * cameraSensitivity);
			}
			if (procEvnt.mCode == KeyCode::Up) {
				ctx.mCamera.Rotate({ Math::ToRadians(15.f),0,0 });
			}
			if (procEvnt.mCode == KeyCode::Down) {
				ctx.mCamera.Rotate({ Math::ToRadians(-15.f),0,0 });
			}
			if (procEvnt.mCode == KeyCode::Left) {
				ctx.mCamera.Rotate({ 0,Math::ToRadians(-15.f),0 });
			}
			if (procEvnt.mCode == KeyCode::Right) {
				ctx.mCamera.Rotate({ 0,Math::ToRadians(15.f),0 });
			}
			if (procEvnt.mCode == KeyCode::RightShift) {
				ctx.mCamera.Rotate({ 0,0,Math::ToRadians(-15.f) });
			}
			if (procEvnt.mCode == KeyCode::RightControl) {
				ctx.mCamera.Rotate({ 0,0,Math::ToRadians(15.f) });
			}
		}
	}

	ImGui::Begin("Debug Window");
	ImGui::Text("Test");
	ImGui::Separator();
	ImGui::End();


	const Vec2i& viewportSize = ctx.mGraphicsContext->GetSwapChainSize();

	ctx.mCamera.SetViewSize({ static_cast<float>(viewportSize.X), static_cast<float>(viewportSize.Y) });
	ctx.mCamera.LookAt({ 0,100,0 });

	ctx.mImGui->Update(*ctx.mGraphics);
	if(!isSuspended) {
		//CommandBuffer& cmdCompute = *ctx.mComputeCommandBuffer;
		//cmdCompute.Begin();
		//cmdCompute.SetPipeline(*ctx.mComputePipeline);
		//cmdCompute.Dispatch(16, 16, 1);
		//cmdCompute.End();

		CommandBuffer& cmd = ctx.mGraphicsContext->BeginCommands();
		{
			cmd.Clear(Color::DarkRed);
			cmd.SetViewport(viewportSize);
			cmd.SetScissor(viewportSize);

			ctx.mGraphicsContext->BeginSwapChainRenderPass(cmd);
			cmd.SetPipeline(*ctx.mPipeline);

			for (int i = 0; i < ctx.mModel->GetMeshes().size(); ++i) {
				auto& mesh = *ctx.mModel->GetMeshes()[i];
				VertexShaderCBData ubo = { ctx.mCamera.GetProjectionMatrix() * ctx.mCamera.GetViewMatrix() * mesh.GetTransform(),  mesh.GetTransform() };
				cmd.UpdateBuffer(*ctx.mMeshResources[i].mUniformBuffer, &ubo, sizeof(ubo));
				cmd.SetPipelineBinding(*ctx.mMeshResources[i].mBinding);
				Buffer* vertexBuff[2] = { mesh.GetVertexBuffer(), ctx.mInstanceBuffer.get() };
				cmd.SetVertexBuffers(vertexBuff, 2);
				cmd.SetIndexBuffer(*mesh.GetIndexBuffer());
				cmd.DrawInstancedIndexed(mesh.GetIndexBuffer()->GetCount(), instanceM * instanceN);
			}

			ctx.mImGui->Draw(cmd);
			ctx.mGraphicsContext->EndSwapChainRenderPass(cmd);
		}
		ctx.mGraphicsContext->EndCommands();
		ctx.mGraphicsContext->Submit(cmd);
		ctx.mGraphicsContext->Present();
	}

	return true;
}
