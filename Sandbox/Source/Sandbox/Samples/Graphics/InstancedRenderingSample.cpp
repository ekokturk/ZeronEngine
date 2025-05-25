// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Sandbox/Samples/Graphics/InstancedRenderingSample.h>

#include <Core/FileSystem.h>
#include <Graphics/Buffer.h>
#include <Graphics/CommandBuffer.h>
#include <Graphics/FrameBuffer.h>
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsContext.h>
#include <Graphics/Pipeline.h>
#include <Graphics/PipelineBinding.h>
#include <Graphics/RenderPass.h>
#include <Graphics/Sampler.h>
#include <Graphics/ShaderProgram.h>
#include <Graphics/ShaderProgramConfig.h>
#include <Graphics/Texture.h>
#include <imgui/imgui.h>
#include <Platform/Asset/Image.h>
#include <Render/Camera/Camera.h>
#include <Render/Light/Light.h>
#include <Render/Mesh/Mesh.h>
#include <Render/Mesh/Model.h>
#include <Render/Mesh/PrimitiveFactory.h>

using namespace Zeron;

namespace Sandbox
{
	InstancedRenderingSample::InstancedRenderingSample(IWindowSampleContext& ctx)
		: IWindowSample(ctx)
	{}

	InstancedRenderingSample::~InstancedRenderingSample() = default;

	void InstancedRenderingSample::Init()
	{
		auto gfx = &mCtx.GetGraphics();

		mShader = mCtx.LoadShader("Standard");

		auto invalidLight = Render::Light();
		invalidLight.mType = Render::Light::Invalid;
		auto pointLight = Render::Light();
		pointLight.mType = Render::Light::Directional;
		pointLight.mDirection = { -0.2f, -0.5f, 0.5f, 1.f };
		mLightList.push_back(pointLight);
		mLightList.push_back(invalidLight);
		auto spotLight = Render::Light();
		spotLight.mType = Render::Light::Spot;
		spotLight.mPosition = Vec4{ 0.f, 2.f, -2.f, 0.f };
		spotLight.mDirection = Vec4{ 0.f, 0.f, -1.f, 0.f };
		spotLight.mSpotAngle = Vec2{ 0.f, 30.f };
		mLightList.push_back(spotLight);
		// Allocate light buffer with additional space
		mLightListBuffer = gfx->CreateBuffer(Gfx::BufferType::Storage, 100, sizeof(Render::Light), nullptr, Gfx::BufferUsageType::Dynamic);
		mLightListBuffer->SetDebugName("Instanced Sample - Light List Buffer");
		mLightingInfo.mLightCount = mLightList.size();
		mLightingInfoBuffer = gfx->CreateUniformBuffer<LightingInfo>(mLightingInfo);

		mPipeline = gfx->CreatePipelineGraphics(
			mShader.get(),
			mCtx.GetGraphicsContext().GetSwapChainRenderPass(),
			{ gfx->GetMultiSamplingLevel(), Gfx::PrimitiveTopology::TriangleList, true, Gfx::FaceCullMode::Back }
		);

		mPerCameraBuffer = gfx->CreateUniformBuffer<PerCameraData>(PerCameraData{});

		mMeshInstanceDataBuffer = gfx->CreateBuffer(
			Gfx::BufferType::Storage, mInstanceM * mInstanceN * 5 + 1, sizeof(MeshInstanceData), nullptr, Gfx::BufferUsageType::Dynamic
		);
		mMeshInstanceDataBuffer->SetDebugName("Instanced Sample - Mesh Instance Buffer");


		auto diffuseImageData = Locator::Get<FileSystem>()->ReadBinaryFile(Path{ "Resources/Textures/TestHumanoid_CLR.png", PathType::Assets });
		mDiffuseImage = std::make_unique<Asset::Image>(diffuseImageData.Value());
		mDiffuseTexture = gfx->CreateTexture({ mDiffuseImage->GetWidth(), mDiffuseImage->GetHeight() }, Gfx::TextureFormat::RGBA_8U, mDiffuseImage->GetRawColor().data());

		auto normalImageData = Locator::Get<FileSystem>()->ReadBinaryFile(Path{ "Resources/Textures/TestHumanoid_NRM.png", PathType::Assets });
		mNormalImage = std::make_unique<Asset::Image>(normalImageData.Value());
		mNormalTexture = gfx->CreateTexture({ mNormalImage->GetWidth(), mNormalImage->GetHeight() }, Gfx::TextureFormat::RGBA_8U, mNormalImage->GetRawColor().data());

		mSampler = gfx->CreateSampler();

		auto modelBuffer = Locator::Get<FileSystem>()->ReadBinaryFile(Path{ "Resources/Models/TestHumanoid_Model.gltf", PathType::Assets });
		mModel = std::make_unique<Render::Model>(*gfx, modelBuffer.Value());

		mShadowShader = mCtx.LoadShader("ShadowMap");
		mShadowMVPBuffer = gfx->CreateUniformBuffer(Mat4{});
		Gfx::RenderPassAttachment shadowAttachment{
			Gfx::TextureFormat::DepthStencil_32U,
			Gfx::TextureLayout::DepthStencilAttachment,
			Gfx::TextureLayout::ShaderReadOnly,
		};
		mShadowRenderPass = gfx->CreateRenderPass({}, std::move(shadowAttachment), Gfx::MSAALevel::Disabled);
		mShadowPipeline = gfx->CreatePipelineGraphics(
			mShadowShader.get(), mShadowRenderPass.get(), { Gfx::MSAALevel::Disabled, Gfx::PrimitiveTopology::TriangleList, true, Gfx::FaceCullMode::Back }
		);
		_createFrameResources();


		MeshComponent meshComp;
		for (size_t i = 0; i < mModel->GetMeshCount(); ++i) {
			meshComp.mMeshes.push_back(mModel->GetMesh(i));
			meshComp.mTransforms.push_back(mModel->GetMeshTransform(i));
		}

		for (int i = 0; i < mInstanceM; ++i) {
			for (int j = 0; j < mInstanceN; ++j) {
				auto entity = mEntityManager.CreateEntity();
				mEntityManager.AddComponent<MeshComponent>(entity, meshComp);
				mEntityManager.AddComponent<TransformComponent>(
					entity, TransformComponent{ Math::Translate(Mat4{}, Vec3{ static_cast<float>(i * 3), 0, static_cast<float>(j * 3) }) }
				);
			}
		}

		mPlane = std::make_unique<Render::Mesh>(mCtx.GetGraphics(), Render::PrimitiveFactory::CreatePlane(20, 20));
		auto entity = mEntityManager.CreateEntity();
		mEntityManager.AddComponent<MeshComponent>(entity, MeshComponent{ { mPlane.get() }, { Mat4{} } });
		mEntityManager.AddComponent<TransformComponent>(entity, TransformComponent{ Math::Translate(Mat4{}, Vec3{ 8, 0, 8 }) });
	}

	void InstancedRenderingSample::Tick()
	{
		mMeshRenderBatch.clear();
		mEntityManager.ForEach<MeshComponent, TransformComponent>([&](Entity e, MeshComponent& meshComponent, TransformComponent& transformComponent) {
			for (size_t i = 0; i < meshComponent.mMeshes.size(); ++i) {
				mMeshRenderBatch[meshComponent.mMeshes[i]].push_back({ meshComponent.mTransforms[i], transformComponent.mTransform });
			}
		});
	}
	void InstancedRenderingSample::OnFrameBegin(Zeron::Gfx::CommandBuffer& cmd)
	{
		cmd.BeginDebugGroup("Instanced Sample - Update Buffers");
		mLightingInfo.mLightCount = mLightList.size();
		cmd.UpdateBuffer(*mLightingInfoBuffer, &mLightingInfo, sizeof(mLightingInfo));
		for (auto& light : mLightList) {
			light.mViewPosition = light.mPosition;
			light.mViewDirection = light.mDirection;
		}
		cmd.UpdateBuffer(*mLightListBuffer, mLightList.data(), sizeof(Render::Light) * mLightList.size());

		std::vector<MeshInstanceData> instanceBuffer;
		for (auto& [mesh, instanceData] : mMeshRenderBatch) {
			instanceBuffer.insert(instanceBuffer.end(), instanceData.begin(), instanceData.end());
		}
		if (!instanceBuffer.empty()) {
			cmd.UpdateBuffer(*mMeshInstanceDataBuffer, instanceBuffer.data(), instanceBuffer.size() * sizeof(MeshInstanceData));
		}
		cmd.EndDebugGroup();

		_doShadowPass(cmd);
	}
	void InstancedRenderingSample::OnFrameEnd(Zeron::Gfx::CommandBuffer& cmd)
	{
		cmd.AddBarrier(*mShadowTexture, Gfx::TextureLayout::ShaderReadOnly, Gfx::TextureLayout::DepthStencilAttachment);
	}

	void InstancedRenderingSample::Draw(Gfx::CommandBuffer& cmd)
	{
		cmd.BeginDebugGroup("Instanced Sample - Render Main");
		{
			cmd.BeginDebugGroup("Prepare Pipeline");
			cmd.SetPipeline(*mPipeline);
			cmd.SetPipelineBinding(*mMeshPipelineBinding);

			PerCameraData ubo = {
				mCtx.GetCamera().GetProjectionMatrix() * mCtx.GetCamera().GetViewMatrix(),
				mCtx.GetCamera().GetViewMatrix(),
				mCtx.GetCamera().GetProjectionMatrix(),
			};
			cmd.UpdateBuffer<PerCameraData>(*mPerCameraBuffer, &ubo);
			cmd.EndDebugGroup();
		}

		const std::array attrs = { Render::MeshAttribute::WorldPos, Render::MeshAttribute::UV, Render::MeshAttribute::Normal, Render::MeshAttribute::Tangent };

		uint32_t instanceOffset = 0;
		for (auto& [mesh, instanceData] : mMeshRenderBatch) {
			cmd.BeginDebugGroup("Draw Mesh");
			mesh->Bind(cmd, std::span{ attrs });
			cmd.SetPushConstant(&instanceOffset, Gfx::ShaderType::Vertex);
			cmd.DrawInstancedIndexed(mesh->GetIndexCount(), instanceData.size());
			instanceOffset += instanceData.size();
			cmd.EndDebugGroup();
		}
		cmd.EndDebugGroup();
	}

	void InstancedRenderingSample::_doShadowPass(Zeron::Gfx::CommandBuffer& cmd)
	{
		if (mLightList.empty() || mLightList[0].mType != Render::Light::Directional || !mLightingInfo.mHasShadows) {
			return;
		}
		cmd.BeginDebugGroup("Instanced Sample - Render Shadows");
		const Vec3 lightDir = Vec3::Normalize({ mLightList[0].mDirection.X, mLightList[0].mDirection.Y, mLightList[0].mDirection.Z });

		const float factor = 20.f;
		const Mat4 view = Math::LookAt(-lightDir, Vec3::ZERO, Math::UP_DIR<float>);
		const Mat4 projection = Math::Orthographic(-factor, factor, -factor, factor, -100.f, mCtx.GetCamera().GetClipPositions().Y);

		mLightingInfo.mShadowMVP = projection * view;
		cmd.UpdateBuffer<Mat4>(*mShadowMVPBuffer, &mLightingInfo.mShadowMVP);

		const std::array attrs = { Render::MeshAttribute::WorldPos };

		cmd.SetScissor(mShadowFrameBuffer->GetExtent());

		cmd.BeginRenderPass(mShadowFrameBuffer.get());
		{
			cmd.SetPipeline(*mShadowPipeline);
			cmd.SetPipelineBinding(*mShadowPipelineBinding);

			uint32_t instanceOffset = 0;
			for (auto& [mesh, instanceData] : mMeshRenderBatch) {
				cmd.BeginDebugGroup("Draw Mesh");
				mesh->Bind(cmd, std::span{ attrs });
				cmd.SetPushConstant(&instanceOffset, Gfx::ShaderType::Vertex);
				cmd.DrawInstancedIndexed(mesh->GetIndexCount(), instanceData.size());
				instanceOffset += instanceData.size();
				cmd.EndDebugGroup();
			}
		}
		cmd.EndRenderPass();

		cmd.AddBarrier(*mShadowTexture, Gfx::TextureLayout::DepthStencilAttachment, Gfx::TextureLayout::ShaderReadOnly);
		cmd.EndDebugGroup();
	}

	void InstancedRenderingSample::OnGui()
	{
		// Light List
		auto& lightList = mLightList;
		if (ImGui::Begin("Instanced Rendering", &mShowingGui)) {
			auto& lightProps = mLightingInfo;
			ImGui::SliderFloat3("Ambient Light Color", reinterpret_cast<float*>(&lightProps.mAmbientLightColor), 0.f, 1.f);
			ImGui::SliderFloat("Ambient Light Strength", &lightProps.mAmbientLightStrength, 0.f, 1.f);
			bool checked = lightProps.mHasShadows == 1;
			if (ImGui::Checkbox("Shadows", &checked)) {
				lightProps.mHasShadows = checked ? 1 : 0;
			}

			if (ImGui::BeginChild("Light List")) {
				const char* lightTypes[] = { "Directional", "Point", "Spot" };

				static int newLightType = static_cast<int>(1);
				ImGui::Combo("Light Type", &newLightType, lightTypes, IM_ARRAYSIZE(lightTypes));
				if (ImGui::Button("Add Light")) {
					Render::Light newLight;
					newLight.mType = static_cast<Render::Light::Type>(newLightType + 1);
					lightList.push_back(newLight);
				}
				ImGui::Separator();

				for (int i = 0; i < lightList.size(); ++i) {
					auto& light = lightList[i];
					if (ImGui::TreeNode(("Light " + std::to_string(i)).c_str())) {
						float color[] = { light.mColor.X, light.mColor.Y, light.mColor.Z, light.mColor.W };
						if (ImGui::SliderFloat4("Color", color, 0.f, 5.f)) {
							light.mColor = { color[0], color[1], color[2], color[3] };
						}
						if (light.mType != Render::Light::Point) {
							float direction[] = { light.mDirection.X, light.mDirection.Y, light.mDirection.Z, light.mDirection.W };
							if (ImGui::DragFloat4("Direction", direction, 0.1f)) {
								light.mDirection = { direction[0], direction[1], direction[2], direction[3] };
							}
						}
						if (light.mType != Render::Light::Directional) {
							float position[] = { light.mPosition.X, light.mPosition.Y, light.mPosition.Z, light.mPosition.W };
							if (ImGui::DragFloat4("Position", position, 1.f)) {
								light.mPosition = { position[0], position[1], position[2], position[3] };
							}
						}
						if (light.mType != Render::Light::Directional) {
							float range = light.mRange;
							if (ImGui::DragFloat("Range", &range, 0.5)) {
								light.mRange = range;
							}
						}
						if (light.mType == Render::Light::Spot) {
							float angle[] = { light.mSpotAngle.X, light.mSpotAngle.Y };
							if (ImGui::SliderFloat2("Angle", angle, 0, 360.f)) {
								light.mSpotAngle = { angle[0], angle[1] };
							}
						}

						int selectedIndex = static_cast<int>(light.mType) - 1;
						if (ImGui::Combo("Type", &selectedIndex, lightTypes, IM_ARRAYSIZE(lightTypes))) {
							light.mType = static_cast<Render::Light::Type>(selectedIndex + 1);
						}
						ImGui::TreePop();
					}
				}
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}
	void InstancedRenderingSample::OnWindowResized(const Zeron::Vec2i& size)
	{
		_createFrameResources();
	}

	void InstancedRenderingSample::_createFrameResources()
	{
		// Shadow
		mShadowTexture = mCtx.GetGraphics().CreateTexture(
			mCtx.GetGraphicsContext().GetSwapChainSize(), Gfx::TextureFormat::DepthStencil_32U, nullptr, Gfx::TextureType::Depth
		);
		mShadowPipelineBinding = mCtx.GetGraphics().CreatePipelineBinding(
			*mShadowPipeline, { Gfx::UniformBindingHandle{ mShadowMVPBuffer.get() }, Gfx::StorageBufferBindingHandle{ mMeshInstanceDataBuffer.get() } }
		);
		mShadowFrameBuffer = mCtx.GetGraphics().CreateFrameBuffer(*mShadowRenderPass, mCtx.GetGraphicsContext().GetSwapChainSize(), {}, mShadowTexture.get());

		// Model
		mMeshPipelineBinding = mCtx.GetGraphics().CreatePipelineBinding(
			*mPipeline,
			std::vector<Gfx::BindingHandle>{
				// Vertex
				Gfx::UniformBindingHandle{ mPerCameraBuffer.get() },
				Gfx::StorageBufferBindingHandle{ mMeshInstanceDataBuffer.get() },
				// Fragment
				Gfx::StorageBufferBindingHandle{ mLightListBuffer.get() },
				Gfx::TextureBindingHandle{ mDiffuseTexture.get() },
				Gfx::TextureBindingHandle{ mNormalTexture.get() },
				Gfx::TextureBindingHandle{ mShadowTexture.get() },
				Gfx::SamplerBindingHandle{ mSampler.get() },
				Gfx::UniformBindingHandle{ mLightingInfoBuffer.get() },
			}
		);
	}


}