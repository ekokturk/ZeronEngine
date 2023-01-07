// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <SampleVulkan.h>
#include <SampleVulkan.h>

#include <GUI/ImGui/ImGuiInstance.h>
#include <Renderer/Image.h>
#include <Renderer/Scene/Camera.h>
#include <Renderer/Scene/Model.h>
#include <Platform/Window.h>

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

namespace SampleVulkan {

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

	struct SampleContext {
		bool mIsRunning = true;

		Window* mWindow;

		Graphics* mGraphics;
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
		bool mIsSuspended = false;

		// Compute
		std::unique_ptr<CommandBuffer> mComputeCommandBuffer;
		std::unique_ptr<ShaderProgram> mComputeShader;
		std::unique_ptr<Pipeline> mComputePipeline;

	};

	float cameraSensitivity = 10.f;
	int instanceM = 10;
	int instanceN = 10;

	SampleInstance::SampleInstance(Zeron::Graphics* graphics, Zeron::Window* window)
		: mCtx(std::make_unique<SampleContext>())
	{
		mCtx->mWindow = window;
		mCtx->mGraphics = graphics;
		auto* gfx = mCtx->mGraphics;
		mCtx->mGraphicsContext = gfx->CreateGraphicsContext();
		mCtx->mGraphicsContext->Init(mCtx->mWindow->GetSystemHandle(), mCtx->mWindow->GetSize());
		mCtx->mImGui = std::make_unique<ImGuiInstance>();
		mCtx->mImGui->Init(*gfx, *mCtx->mGraphicsContext);

		std::vector<VertexInstance> instanceData;
		for (int i = 0; i < instanceM; ++i) {
			for (int j = 0; j < instanceN; ++j) {
				instanceData.emplace_back(VertexInstance{ {i * 10.f, j * 10.f, 0 } });
			}
		}

		mCtx->mInstanceBuffer = gfx->CreateVertexBuffer<VertexInstance>(instanceData);

		// Compute
		mCtx->mComputeCommandBuffer = gfx->CreateCommandBuffer(1, true);
		mCtx->mComputeShader = gfx->CreateShaderProgram("ComputeTest", "Resources/Shaders", {}, {});
		mCtx->mComputePipeline = gfx->CreatePipeline(mCtx->mComputeShader.get());


		mCtx->mShader = gfx->CreateShaderProgram("Standard", "Resources/Shaders", {
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

		mCtx->mPipeline = gfx->CreatePipeline(
			mCtx->mShader.get(),
			mCtx->mGraphicsContext->GetSwapChainRenderPass(),
			gfx->GetMultiSamplingLevel(),
			PrimitiveTopology::TriangleList,
			true,
			FaceCullMode::Back
		);

		mCtx->mImage = std::make_unique<Image>();
		mCtx->mImage->Load("Resources/Textures/TestHumanoid_CLR.png");
		mCtx->mTexture = gfx->CreateTexture(TextureType::Diffuse, mCtx->mImage->GetColorData().data(), mCtx->mImage->GetWidth(), mCtx->mImage->GetHeight());

		mCtx->mSampler = gfx->CreateSampler();

		mCtx->mModel = std::make_unique<Model>(*gfx, "Resources/Models/TestHumanoid_Model.fbx", nullptr);
		for (auto& mesh : mCtx->mModel->GetMeshes()) {
			MeshResource res;
			res.mUniformBuffer = gfx->CreateUniformBuffer<VertexShaderCBData>(VertexShaderCBData{});
			res.mLightBuffer = gfx->CreateUniformBuffer<PixelShaderCBData>(PixelShaderCBData{});
			res.mBinding = gfx->CreatePipelineBinding(*mCtx->mPipeline, std::vector<BindingHandle>{
				UniformBindingHandle{ res.mUniformBuffer.get() },
					TextureBindingHandle{ mCtx->mTexture.get() },
					TextureBindingHandle{ mCtx->mTexture.get() },
					SamplerBindingHandle{ mCtx->mSampler.get() },
					UniformBindingHandle{ res.mLightBuffer.get() },
			});
			mCtx->mMeshResources.push_back(std::move(res));
		}

		mCtx->mCamera.SetPosition({ 0.f, 200.f, -400.f });
		mCtx->mCamera.SetFieldOfView(60.f);
	}

	SampleInstance::~SampleInstance()
	{
	}

	bool SampleInstance::Run()
	{
		if (!mCtx->mIsRunning) {
			return false;
		}

		mCtx->mImGui->NewFrame();

		while(mCtx->mWindow->HasSystemEvents()) {
			SystemEvent e = mCtx->mWindow->GetNextSystemEvent();
			if (mCtx->mImGui->HandleEvent(e)) {
				continue;
			}
			std::visit(Visitor{
				[&](const SystemEvent::WindowClosed&) {
					mCtx->mIsRunning = false;
				},
				[&](const SystemEvent::WindowMinimized&) {
					mCtx->mIsSuspended = true;
				},
				[&](const SystemEvent::WindowRestored&) {
					mCtx->mIsSuspended = false;
				},
				[&](const SystemEvent::WindowResized& data) {
					mCtx->mGraphicsContext->ResizeSwapChain(Vec2i(data.mWidth, data.mHeight));
				},
				[&](const SystemEvent::KeyDown& data) {
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
		ImGui::Text("Test");
		ImGui::Separator();
		ImGui::End();


		const Vec2i& viewportSize = mCtx->mGraphicsContext->GetSwapChainSize();

		mCtx->mCamera.SetViewSize({ static_cast<float>(viewportSize.X), static_cast<float>(viewportSize.Y) });
		mCtx->mCamera.LookAt({ 0,100,0 });

		mCtx->mImGui->Update(*mCtx->mGraphics);
		if(!mCtx->mIsSuspended) {
			//CommandBuffer& cmdCompute = *mCtx->mComputeCommandBuffer;
			//cmdCompute.Begin();
			//cmdCompute.SetPipeline(*mCtx->mComputePipeline);
			//cmdCompute.Dispatch(16, 16, 1);
			//cmdCompute.End();

			CommandBuffer& cmd = mCtx->mGraphicsContext->BeginCommands();
			{
				cmd.Clear(Color::DarkRed);
				cmd.SetViewport(viewportSize);
				cmd.SetScissor(viewportSize);

				mCtx->mGraphicsContext->BeginSwapChainRenderPass(cmd);
				cmd.SetPipeline(*mCtx->mPipeline);

				for (int i = 0; i < mCtx->mModel->GetMeshes().size(); ++i) {
					auto& mesh = *mCtx->mModel->GetMeshes()[i];
					VertexShaderCBData ubo = { mCtx->mCamera.GetProjectionMatrix() * mCtx->mCamera.GetViewMatrix() * mesh.GetTransform(),  mesh.GetTransform() };
					cmd.UpdateBuffer(*mCtx->mMeshResources[i].mUniformBuffer, &ubo, sizeof(ubo));
					cmd.SetPipelineBinding(*mCtx->mMeshResources[i].mBinding);
					Buffer* vertexBuff[2] = { mesh.GetVertexBuffer(), mCtx->mInstanceBuffer.get() };
					cmd.SetVertexBuffers(vertexBuff, 2);
					cmd.SetIndexBuffer(*mesh.GetIndexBuffer());
					cmd.DrawInstancedIndexed(mesh.GetIndexBuffer()->GetCount(), instanceM * instanceN);
				}

				mCtx->mImGui->Draw(cmd);
				mCtx->mGraphicsContext->EndSwapChainRenderPass(cmd);
			}
			mCtx->mGraphicsContext->EndCommands();
			mCtx->mGraphicsContext->Submit(cmd);
			mCtx->mGraphicsContext->Present();
		}

		return mCtx->mIsRunning;
	}

}
