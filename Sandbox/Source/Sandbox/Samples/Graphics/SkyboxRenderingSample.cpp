// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Sandbox/Samples/Graphics/SkyboxRenderingSample.h>

#include <Core/FileSystem.h>
#include <Graphics/Buffer.h>
#include <Graphics/CommandBuffer.h>
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsContext.h>
#include <Graphics/Pipeline.h>
#include <Graphics/PipelineBinding.h>
#include <Graphics/Sampler.h>
#include <Graphics/ShaderProgram.h>
#include <Graphics/ShaderProgramConfig.h>
#include <imgui/imgui.h>
#include <Render/Camera/Camera.h>
#include <Render/Font/FontAtlas.h>
#include <Render/Mesh/MeshData.h>
#include <Render/Mesh/Model.h>
#include <Render/Mesh/PrimitiveFactory.h>

using namespace Zeron;

namespace Sandbox
{
	SkyboxRenderingSample::SkyboxRenderingSample(IWindowSampleContext& ctx)
		: IWindowSample(ctx)
	{}

	SkyboxRenderingSample::~SkyboxRenderingSample() = default;

	void SkyboxRenderingSample::Init()
	{
		mSkyboxMesh = std::make_unique<Render::Mesh>(mCtx.GetGraphics(), Render::PrimitiveFactory::CreateSphere(1000));

		mShader = mCtx.LoadShader("Skybox");

		Mat4 viewWithNoTranslation = mCtx.GetCamera().GetViewMatrix();
		viewWithNoTranslation[3] = { 0.f, 0.f, 0.f, 1.f };
		Mat4 ubo[2] = { mCtx.GetCamera().GetViewMatrix(), viewWithNoTranslation * mCtx.GetCamera().GetProjectionMatrix() };
		mSettingsBuffer = mCtx.GetGraphics().CreateUniformBuffer(mSkyboxSettings);

		mPipeline = mCtx.GetGraphics().CreatePipelineGraphics(
			mShader.get(),
			mCtx.GetGraphicsContext().GetSwapChainRenderPass(),
			{ mCtx.GetGraphics().GetMultiSamplingLevel(), Gfx::PrimitiveTopology::TriangleList, true, Gfx::FaceCullMode::Front, Gfx::DepthMode::Default }

		);
		mPipelineBinding = mCtx.GetGraphics().CreatePipelineBinding(
			*mPipeline,
			{ {
				Gfx::UniformBindingHandle{ mSettingsBuffer.get() },
			} }
		);
	}

	void SkyboxRenderingSample::Draw(Gfx::CommandBuffer& cmd)
	{
		cmd.BeginDebugGroup("Skybox Sample - Render");
		cmd.UpdateBuffer<SkyboxSettings>(*mSettingsBuffer, &mSkyboxSettings);
		cmd.SetPipeline(*mPipeline);
		cmd.SetPipelineBinding(*mPipelineBinding);
		const Mat4 ubo[2] = { mCtx.GetCamera().GetViewMatrix(), mCtx.GetCamera().GetProjectionMatrix() };
		cmd.SetPushConstant(ubo, sizeof(Mat4) * 2, Gfx::ShaderType::Vertex);
		const std::array attrs = { Render::MeshAttribute::WorldPos };
		mSkyboxMesh->Bind(cmd, attrs);
		cmd.DrawIndexed(mSkyboxMesh->GetIndexCount());
		cmd.EndDebugGroup();
	}

	void SkyboxRenderingSample::OnGui()
	{

		if (ImGui::Begin("Skybox Settings", &mShowingGui)) {
			ImGui::SliderFloat3("Sun Direction", &mSkyboxSettings.mSunDirection.X, -1.0f, 1.0f);
			ImGui::ColorEdit3("Sky Color", &mSkyboxSettings.mSkyColor.X);
			ImGui::ColorEdit3("Horizon Color", &mSkyboxSettings.mHorizonColor.X);
			ImGui::ColorEdit3("Ground Color", &mSkyboxSettings.mGroundColor.X);
			ImGui::SliderFloat("Atmosphere Intensity", &mSkyboxSettings.mAtmosphereIntensity, 0.0f, 1.0f);
		}
		ImGui::End();
	}

}