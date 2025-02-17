// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Sandbox/Samples/Graphics/WireframePhysicsSample.h>

#include <Graphics/CommandBuffer.h>
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsContext.h>
#include <Graphics/Pipeline.h>
#include <Graphics/PipelineBinding.h>
#include <Graphics/ShaderProgram.h>
#include <Graphics/ShaderProgramConfig.h>
#include <imgui/imgui.h>
#include <Physics/DynamicBody.h>
#include <Physics/Engine.h>
#include <Physics/Material.h>
#include <Physics/World.h>
#include <Render/Camera/Camera.h>
#include <Render/Mesh/Mesh.h>
#include <Render/Mesh/PrimitiveFactory.h>

using namespace Zeron;

namespace Sandbox
{
	WireframePhysicsSample::WireframePhysicsSample(IWindowSampleContext& ctx)
		: IWindowSample(ctx)
	{}

	WireframePhysicsSample::~WireframePhysicsSample() {}

	void WireframePhysicsSample::Init()
	{
		mShader = mCtx.LoadShader("Wireframe");

		mPhysics = std::make_unique<Physics::Engine>();
		mPhysicsWorld = mPhysics->CreateWorld(Physics::WorldSettings{});

		mDynamicMaterial = mPhysics->CreateMaterial(0.5, 0.5, .4f);
		const Physics::DynamicBodyConfig dynamicConfig{
			.mTransform = Math::MakeTransform(Vec3(0, 100, 0), Math::ToRadians(Vec3{ 40.f, 50.f, 0.f })),
			.mMaterial = mDynamicMaterial.get(),
			.mShape = Physics::BoxShape{ Vec3{ 0.5f, 0.5f, 0.5f } },
		};
		mDynamicBody = mPhysics->CreateDynamicBody(dynamicConfig);
		mDebugMesh = std::make_unique<Render::Mesh>(mCtx.GetGraphics(), Render::PrimitiveFactory::CreateCube());

		const Physics::CollisionBodyConfig staticConfig = { .mTransform = Mat4(), .mShape = Physics::BoxShape{ Vec3{ 10.f, 0.5f, 10.f } } };
		mStaticBody = mPhysics->CreateStaticBody(staticConfig);
		mPhysicsWorld->AddBody(*mStaticBody);
		mPhysicsWorld->AddBody(*mDynamicBody);

		mPipeline = mCtx.GetGraphics().CreatePipelineGraphics(
			mShader.get(),
			mCtx.GetGraphicsContext().GetSwapChainRenderPass(),
			{ mCtx.GetGraphics().GetMultiSamplingLevel(), Gfx::PrimitiveTopology::TriangleList, false, Gfx::FaceCullMode::Front, Gfx::DepthMode::Default }
		);
	}
	void WireframePhysicsSample::Tick()
	{
		mPhysicsWorld->Simulate(1 / 60.f);
	}

	void WireframePhysicsSample::Draw(Gfx::CommandBuffer& cmd)
	{
		cmd.BeginDebugGroup("Wireframe Physics Sample - Render");
		cmd.SetPipeline(*mPipeline);
		const struct PC {
			Mat4 MPV;
			std::array<float, 4> mColor;
		} pc = { mCtx.GetCamera().GetViewProjectionMatrix() * mDynamicBody->GetTransform(), Color(Color::Green).norm() };
		cmd.SetPushConstant(&pc, sizeof(PC), Gfx::ShaderType::Vertex);
		const std::array attrs = { Render::MeshAttribute::WorldPos };
		mDebugMesh->Bind(cmd, attrs);
		cmd.DrawIndexed(mDebugMesh->GetIndexCount());
		cmd.EndDebugGroup();
	}

	void WireframePhysicsSample::OnGui()
	{

		if (ImGui::Begin("Wireframe Physics Settings", &mShowingGui)) {
			Physics::BoxShape overlapShape{ Vec3{ 10.f, 10.f, 10.f } };
			const auto overlapList = mPhysicsWorld->Overlap(overlapShape, Math::Translate(Mat4(), Vec3(0.f, 10.f, 0.f)));
			const std::string text = Util::Format("Overlap Count: {}", overlapList.size());
			ImGui::Text(text.c_str());
		}
		ImGui::End();
	}

}