// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Samples/SampleGraphics.h>

#include <GUI/ImGui/ImGuiInstance.h>
#include <Platform/FileSystem.h>
#include <Platform/Window.h>
#include <Render/Camera/Camera.h>
#include <Render/Image.h>
#include <Render/Mesh/Model.h>

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
#include <Render/Font/FontAtlas.h>
#include <Render/Mesh/MeshInfo.h>

using namespace Zeron;

namespace SampleGraphics
{
	struct TextRenderExample;
	struct InstancedRenderExample;

	struct VertexShaderCBData {
		Mat4 mViewModelProjectionMatrix;
		Mat4 mWorldMatrix;
	};

	struct SampleContext {
		bool mIsSuspended = false;

		Window* mWindow;

		Gfx::Graphics* mGraphics;
		std::unique_ptr<Gfx::GraphicsContext> mGraphicsContext;
		std::unique_ptr<ImGuiInstance> mImGui;

		Render::Camera mCamera;
		float mCameraSensitivity = 10.f;
		bool mIsCameraLookAtEnabled = false;

		// Compute
		std::unique_ptr<Gfx::CommandBuffer> mComputeCommandBuffer;
		std::unique_ptr<Gfx::ShaderProgram> mComputeShader;
		std::unique_ptr<Gfx::Pipeline> mComputePipeline;

		// Font
		std::unique_ptr<Render::FontAtlas> mFont;

		// Examples
		std::unique_ptr<InstancedRenderExample> mInstancedRenderExample;
		std::unique_ptr<TextRenderExample> mTextRenderExample;
	};

	struct InstancedRenderExample {
		struct MeshResource {
			std::unique_ptr<Gfx::Buffer> mUniformBuffer;
			std::unique_ptr<Gfx::Buffer> mLightBuffer;
			std::unique_ptr<Gfx::PipelineBinding> mBinding;
		};

		struct LightingProps {
			// float mAlpha = 1.f;
			Vec3 mAmbientLightColor = Vec3::ONE;
			float mAmbientLightStrength = 1.f;

			Vec3 mDynamicLightColor = Vec3::ONE;
			float mDynamicLightStrength = 1.f;
			Vec3 mDynamicLightPosition;
			float mDynamicLightAttenuationA = 1.f;
			float mDynamicLightAttenuationB = 0.1f;
			float mDynamicLightAttenuationC = 0.1f;
		} mLightProps;

		std::unique_ptr<Gfx::Sampler> mSampler;
		std::unique_ptr<Gfx::Buffer> mInstanceBuffer;
		std::unique_ptr<Gfx::ShaderProgram> mShader;
		std::unique_ptr<Gfx::Texture> mTexture;
		std::unique_ptr<Gfx::Pipeline> mPipeline;

		std::vector<MeshResource> mMeshResources;

		std::unique_ptr<Render::Model> mModel;
		std::unique_ptr<Render::Image> mImage;

		int mInstanceM = 10;
		int mInstanceN = 10;

		InstancedRenderExample(SampleContext& ctx)
		{
			auto gfx = ctx.mGraphics;


			auto vertexShaderBuffer = FileSystem::ReadBinaryFile(Path("Resources/Shaders") / gfx->GetCompiledShaderName("Standard", Gfx::ShaderType::Vertex));
			auto fragmentShaderBuffer = FileSystem::ReadBinaryFile(Path("Resources/Shaders") / gfx->GetCompiledShaderName("Standard", Gfx::ShaderType::Fragment));
			mShader = gfx->CreateShaderProgram(
				"Standard",
				{
					{ "POSITION", Gfx::VertexFormat::Float3 },
					{ "TEXTURE_COORD", Gfx::VertexFormat::Float2 },
					{ "NORMAL", Gfx::VertexFormat::Float3 },
					{ "INSTANCE_POS", Gfx::VertexFormat::Float3, true, 1 },
				},
				{
					{ Gfx::PipelineResourceType::UniformBuffer, Gfx::ShaderType::Vertex, 0 },
					{ Gfx::PipelineResourceType::Texture, Gfx::ShaderType::Fragment, 1 },
					{ Gfx::PipelineResourceType::Texture, Gfx::ShaderType::Fragment, 2 },
					{ Gfx::PipelineResourceType::Sampler, Gfx::ShaderType::Fragment, 3 },
					{ Gfx::PipelineResourceType::UniformBuffer, Gfx::ShaderType::Fragment, 4 },
				},
				vertexShaderBuffer.Value(),
				fragmentShaderBuffer.Value()
			);

			mPipeline = gfx->CreatePipeline(
				mShader.get(),
				ctx.mGraphicsContext->GetSwapChainRenderPass(),
				gfx->GetMultiSamplingLevel(),
				Gfx::PrimitiveTopology::TriangleList,
				true,
				Gfx::FaceCullMode::Back
			);

			std::vector<Gfx::VertexInstance> instanceData;
			for (int i = 0; i < mInstanceM; ++i) {
				for (int j = 0; j < mInstanceN; ++j) {
					instanceData.emplace_back(Gfx::VertexInstance{ { i * 10.f, j * 10.f, 0 } });
				}
			}

			mInstanceBuffer = gfx->CreateVertexBuffer<Gfx::VertexInstance>(instanceData);

			mImage = std::make_unique<Render::Image>();
			auto imageBuffer = FileSystem::ReadBinaryFile("Resources/Textures/TestHumanoid_CLR.png");
			mImage->Load(imageBuffer.Value());
			mTexture = gfx->CreateTexture(Gfx::TextureType::Diffuse, mImage->GetColorData().data(), mImage->GetWidth(), mImage->GetHeight());

			mSampler = gfx->CreateSampler();

			auto modelBuffer = FileSystem::ReadBinaryFile("Resources/Models/TestHumanoid_Model.fbx");
			mModel = std::make_unique<Render::Model>(*gfx, modelBuffer.Value());
			for (auto& mesh : mModel->GetMeshes()) {
				MeshResource res;
				res.mUniformBuffer = gfx->CreateUniformBuffer<VertexShaderCBData>(VertexShaderCBData{});
				res.mLightBuffer = gfx->CreateUniformBuffer<LightingProps>(LightingProps{});
				res.mBinding = gfx->CreatePipelineBinding(
					*mPipeline,
					std::vector<Gfx::BindingHandle>{
						Gfx::UniformBindingHandle{ res.mUniformBuffer.get() },
						Gfx::TextureBindingHandle{ mTexture.get() },
						Gfx::TextureBindingHandle{ mTexture.get() },
						Gfx::SamplerBindingHandle{ mSampler.get() },
						Gfx::UniformBindingHandle{ res.mLightBuffer.get() },
					}
				);
				mMeshResources.push_back(std::move(res));
			}
		}

		void Draw(SampleContext& ctx, Gfx::CommandBuffer& cmd)
		{
			cmd.SetPipeline(*mPipeline);
			for (size_t i = 0; i < mModel->GetMeshCount(); ++i) {
				const Mesh& mesh = *mModel->GetMesh(i);
				const Mat4& meshTransform = mModel->GetMeshTransform(i);
				cmd.UpdateBuffer(*mMeshResources[i].mLightBuffer, &mLightProps, sizeof(mLightProps));
				VertexShaderCBData ubo = { ctx.mCamera.GetProjectionMatrix() * ctx.mCamera.GetViewMatrix() * meshTransform, meshTransform };
				cmd.UpdateBuffer(*mMeshResources[i].mUniformBuffer, &ubo, sizeof(ubo));
				cmd.SetPipelineBinding(*mMeshResources[i].mBinding);
				Gfx::Buffer* vertexBuff[2] = { mesh.GetVertexBuffer(), mInstanceBuffer.get() };
				cmd.SetVertexBuffers(vertexBuff, 2);
				cmd.SetIndexBuffer(*mesh.GetIndexBuffer());
				cmd.DrawInstancedIndexed(mesh.GetIndexBuffer()->GetCount(), mInstanceM * mInstanceN);
			}
		}
	};

	struct TextRenderExample {
		Render::MeshInfo mTextMeshInfo;

		std::unique_ptr<Gfx::Buffer> mVertexBuffer;
		std::unique_ptr<Gfx::Buffer> mUVBuffer;
		std::unique_ptr<Gfx::Buffer> mIndexBuffer;

		std::unique_ptr<Gfx::Sampler> mSampler;
		std::unique_ptr<Gfx::ShaderProgram> mShader;
		std::unique_ptr<Gfx::Pipeline> mPipeline;
		std::unique_ptr<Gfx::PipelineBinding> mWorldBinding;
		std::unique_ptr<Gfx::PipelineBinding> mScreenBinding;
		std::unique_ptr<Gfx::Buffer> mMVPWorldBuffer;
		std::unique_ptr<Gfx::Buffer> mMVPScreenBuffer;
		std::unique_ptr<Gfx::Buffer> mColorBuffer;

		TextRenderExample(SampleContext& ctx, const std::string& text)
		{
			auto vertexShaderBuffer = FileSystem::ReadBinaryFile(Path("Resources/Shaders") / ctx.mGraphics->GetCompiledShaderName("Basic2D", Gfx::ShaderType::Vertex));
			auto fragmentShaderBuffer = FileSystem::ReadBinaryFile(Path("Resources/Shaders") / ctx.mGraphics->GetCompiledShaderName("Basic2D", Gfx::ShaderType::Fragment));
			mShader = ctx.mGraphics->CreateShaderProgram(
				"Standard",
				{
					{ "POSITION", Gfx::VertexFormat::Float2, false, 0 },
					{ "TEXCOORD", Gfx::VertexFormat::Float2, false, 1 },
				},
				{
					{ Gfx::PipelineResourceType::UniformBuffer, Gfx::ShaderType::Vertex, 0 },
					{ Gfx::PipelineResourceType::Texture, Gfx::ShaderType::Fragment, 1 },
					{ Gfx::PipelineResourceType::Sampler, Gfx::ShaderType::Fragment, 2 },
					{ Gfx::PipelineResourceType::UniformBuffer, Gfx::ShaderType::Fragment, 3 },
				},
				vertexShaderBuffer.Value(),
				fragmentShaderBuffer.Value()
			);

			mSampler = ctx.mGraphics->CreateSampler();
			mMVPWorldBuffer = ctx.mGraphics->CreateUniformBuffer(Mat4{});
			mMVPScreenBuffer = ctx.mGraphics->CreateUniformBuffer(Mat4{});
			mColorBuffer = ctx.mGraphics->CreateUniformBuffer(Color(Color::Palette::White).norm());

			mPipeline = ctx.mGraphics->CreatePipeline(
				mShader.get(),
				ctx.mGraphicsContext->GetSwapChainRenderPass(),
				ctx.mGraphics->GetMultiSamplingLevel(),
				Gfx::PrimitiveTopology::TriangleList,
				true,
				Gfx::FaceCullMode::None
			);
			mWorldBinding = ctx.mGraphics->CreatePipelineBinding(
				*mPipeline,
				{ {
					Gfx::UniformBindingHandle{ mMVPWorldBuffer.get() },
					Gfx::TextureBindingHandle{ ctx.mFont->GetTexture() },
					Gfx::SamplerBindingHandle{ mSampler.get() },
					Gfx::UniformBindingHandle{ mColorBuffer.get() },
				} }
			);
			mScreenBinding = ctx.mGraphics->CreatePipelineBinding(
				*mPipeline,
				{ {
					Gfx::UniformBindingHandle{ mMVPScreenBuffer.get() },
					Gfx::TextureBindingHandle{ ctx.mFont->GetTexture() },
					Gfx::SamplerBindingHandle{ mSampler.get() },
					Gfx::UniformBindingHandle{ mColorBuffer.get() },
				} }
			);

			mTextMeshInfo = ctx.mFont->GenerateText(text);
			const auto& positionAttr = mTextMeshInfo.GetAttribute<Render::MeshAttribute::Position>();
			const auto& uvAttr = mTextMeshInfo.GetAttribute<Render::MeshAttribute::UV>();
			const auto& indexAttr = mTextMeshInfo.GetAttribute<Render::MeshAttribute::Index>();

			mVertexBuffer = ctx.mGraphics->CreateBuffer(Gfx::BufferType::Vertex, positionAttr.GetSize(), positionAttr.GetStride(), positionAttr.GetData());
			mUVBuffer = ctx.mGraphics->CreateBuffer(Gfx::BufferType::Vertex, uvAttr.GetSize(), uvAttr.GetStride(), uvAttr.GetData());
			mIndexBuffer = ctx.mGraphics->CreateBuffer(Gfx::BufferType::Index, indexAttr.GetSize(), indexAttr.GetStride(), indexAttr.GetData());
		}

		void Draw(SampleContext& ctx, Gfx::CommandBuffer& cmd)
		{
			cmd.SetPipeline(*mPipeline);
			Gfx::Buffer* vertexBuff[2] = { mVertexBuffer.get(), mUVBuffer.get() };
			cmd.SetVertexBuffers(vertexBuff, 2);
			cmd.SetIndexBuffer(*mIndexBuffer);

			Mat4 ubo = { ctx.mCamera.GetProjectionMatrix() * ctx.mCamera.GetViewMatrix() * glm::translate(glm::mat4(1.0f), { -50.f, 250.f, 0.f }) };
			ubo = Math::Rotate(ubo, Math::PI<float>(), Math::RIGHT_DIR<float>);
			cmd.UpdateBuffer<Mat4>(*mMVPWorldBuffer, &ubo);
			cmd.SetPipelineBinding(*mWorldBinding);

			cmd.DrawIndexed(mIndexBuffer->GetCount());

			const Mat4 proj = Math::Orthographic(0.f, 800.f, 600.f, 0.f, .0f, 1.f);
			cmd.UpdateBuffer<Mat4>(*mMVPScreenBuffer, &proj);
			cmd.SetPipelineBinding(*mScreenBinding);

			cmd.DrawIndexed(mIndexBuffer->GetCount());
		}
	};


	SampleInstance::SampleInstance(Gfx::Graphics* graphics, Zeron::Window* window)
		: mCtx(std::make_unique<SampleContext>())
	{
		mCtx->mWindow = window;
		mCtx->mGraphics = graphics;
	}

	SampleInstance::~SampleInstance() = default;

	void SampleInstance::Init()
	{
		mIsInit = true;
		mIsRunning = true;

		auto* gfx = mCtx->mGraphics;
		mCtx->mGraphicsContext = gfx->CreateGraphicsContext();
		mCtx->mGraphicsContext->Init(mCtx->mWindow->GetSystemHandle(), mCtx->mWindow->GetSize());
		mCtx->mImGui = std::make_unique<ImGuiInstance>();
		mCtx->mImGui->Init(*gfx, *mCtx->mGraphicsContext);

		auto font = FileSystem::ReadBinaryFile(Path("Resources/Fonts/NimbusSans.ttf"));
		ZE_ASSERT(!font.HasError(), "");
		mCtx->mFont = std::make_unique<Render::FontAtlas>("Arial");
		mCtx->mFont->Load(*mCtx->mGraphics, font.Value());

		// Examples
		mCtx->mInstancedRenderExample = std::make_unique<InstancedRenderExample>(*mCtx);
		mCtx->mTextRenderExample = std::make_unique<TextRenderExample>(*mCtx, "Test String!");

		mCtx->mCamera.SetPosition({ 0.f, 200.f, -400.f });
		mCtx->mCamera.SetFieldOfView(60.f);
	}

	void SampleInstance::Update()
	{
		if (!mIsRunning) {
			return;
		}

		mCtx->mImGui->NewFrame();

		while (mCtx->mWindow->HasSystemEvents()) {
			SystemEvent e = mCtx->mWindow->GetNextSystemEvent();
			if (mCtx->mImGui->HandleEvent(e)) {
				continue;
			}
			std::visit(
				TypeTraits::Visitor{
					[&](const SystemEvent::WindowClosed&) {
						mIsRunning = false;
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
							mCtx->mCamera.Move(mCtx->mCamera.GetForwardDir() * mCtx->mCameraSensitivity);
						}
						if (data.mCode == KeyCode::S) {
							mCtx->mCamera.Move(-mCtx->mCamera.GetForwardDir() * mCtx->mCameraSensitivity);
						}
						if (data.mCode == KeyCode::A) {
							mCtx->mCamera.Move(-mCtx->mCamera.GetRightDir() * mCtx->mCameraSensitivity);
						}
						if (data.mCode == KeyCode::D) {
							mCtx->mCamera.Move(mCtx->mCamera.GetRightDir() * mCtx->mCameraSensitivity);
						}
						if (data.mCode == KeyCode::Q) {
							mCtx->mCamera.Move(mCtx->mCamera.GetUpDir() * mCtx->mCameraSensitivity);
						}
						if (data.mCode == KeyCode::E) {
							mCtx->mCamera.Move(-mCtx->mCamera.GetUpDir() * mCtx->mCameraSensitivity);
						}
						if (data.mCode == KeyCode::Up) {
							mCtx->mCamera.Rotate({ Math::ToRadians(15.f), 0, 0 });
						}
						if (data.mCode == KeyCode::Down) {
							mCtx->mCamera.Rotate({ Math::ToRadians(-15.f), 0, 0 });
						}
						if (data.mCode == KeyCode::Left) {
							mCtx->mCamera.Rotate({ 0, Math::ToRadians(-15.f), 0 });
						}
						if (data.mCode == KeyCode::Right) {
							mCtx->mCamera.Rotate({ 0, Math::ToRadians(15.f), 0 });
						}
						if (data.mCode == KeyCode::RightShift) {
							mCtx->mCamera.Rotate({ 0, 0, Math::ToRadians(-15.f) });
						}
						if (data.mCode == KeyCode::RightControl) {
							mCtx->mCamera.Rotate({ 0, 0, Math::ToRadians(15.f) });
						}
					},
					[&](const SystemEvent::MouseScroll& data) {
						if (data.mOffsetY > 0) {
							mCtx->mCamera.Move(mCtx->mCamera.GetForwardDir() * mCtx->mCameraSensitivity);
						}
						else {
							mCtx->mCamera.Move(-mCtx->mCamera.GetForwardDir() * mCtx->mCameraSensitivity);
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

		ImGui::Begin("Debug Window");
		{
			auto& lightProps = mCtx->mInstancedRenderExample->mLightProps;
			ImGui::SliderFloat3("Ambient Light Color", reinterpret_cast<float*>(&lightProps.mAmbientLightColor), 0.f, 1.f);
			ImGui::SliderFloat("Ambient Light Strength", &lightProps.mAmbientLightStrength, 0.f, 1.f);
			ImGui::Separator();
			ImGui::DragFloat3("Dynamic Light Position", reinterpret_cast<float*>(&lightProps.mDynamicLightPosition), 3.f);
			ImGui::SliderFloat("Dynamic Light  Strength", &lightProps.mDynamicLightStrength, 0.f, 100.f);
			ImGui::SliderFloat("Attenuation A", &lightProps.mDynamicLightAttenuationA, 0.f, 10.f);
			ImGui::SliderFloat("Attenuation B", &lightProps.mDynamicLightAttenuationB, 0.f, 10.f);
			ImGui::SliderFloat("Attenuation C", &lightProps.mDynamicLightAttenuationC, 0.f, 10.f);
			ImGui::Separator();
		}

		float rotation[] = { mCtx->mCamera.GetRotation().X, mCtx->mCamera.GetRotation().Y, mCtx->mCamera.GetRotation().Z };
		if (ImGui::SliderFloat3("Rotation", rotation, -Math::PI<float>(), Math::PI<float>())) {
			mCtx->mCamera.SetRotation({ rotation[0], rotation[1], rotation[2] });
		}
		float position[] = { mCtx->mCamera.GetPosition().X, mCtx->mCamera.GetPosition().Y, mCtx->mCamera.GetPosition().Z };
		if (ImGui::DragFloat3("Position", position, -20, 20)) {
			mCtx->mCamera.SetPosition({ position[0], position[1], position[2] });
		}
		ImGui::Checkbox("Camera Look At", &mCtx->mIsCameraLookAtEnabled);
		bool projPers = mCtx->mCamera.GetProjectionType() == Render::Camera::ProjectionType::Perspective;
		if (ImGui::Checkbox("Is Perspective", &projPers)) {
			mCtx->mCamera.SetProjectionType(projPers ? Render::Camera::ProjectionType::Perspective : Render::Camera::ProjectionType::Orthographic);
		}
		ImGui::End();


		const Vec2i& viewportSize = mCtx->mGraphicsContext->GetSwapChainSize();

		mCtx->mCamera.SetViewSize({ static_cast<float>(viewportSize.X), static_cast<float>(viewportSize.Y) });
		mCtx->mCamera.LookAt({ 0, 100, 0 });

		mCtx->mImGui->Update(*mCtx->mGraphics);
		if (!mCtx->mIsSuspended) {
			// CommandBuffer& cmdCompute = *mCtx->mComputeCommandBuffer;
			// cmdCompute.Begin();
			// cmdCompute.SetPipeline(*mCtx->mComputePipeline);
			// cmdCompute.Dispatch(16, 16, 1);
			// cmdCompute.End();

			Gfx::CommandBuffer& cmd = mCtx->mGraphicsContext->BeginCommands();
			{
				cmd.Clear(Color::DarkRed);
				cmd.SetViewport(viewportSize);
				cmd.SetScissor(viewportSize);

				mCtx->mGraphicsContext->BeginSwapChainRenderPass(cmd);
				{
					mCtx->mInstancedRenderExample->Draw(*mCtx, cmd);
					mCtx->mTextRenderExample->Draw(*mCtx, cmd);
					mCtx->mImGui->Draw(cmd);
				}
				mCtx->mGraphicsContext->EndSwapChainRenderPass(cmd);
			}
			mCtx->mGraphicsContext->EndCommands();
			mCtx->mGraphicsContext->Submit(cmd);
			mCtx->mGraphicsContext->Present();
		}
	}


}