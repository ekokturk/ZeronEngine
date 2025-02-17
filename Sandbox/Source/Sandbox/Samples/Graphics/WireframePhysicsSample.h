// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Sandbox/Samples/IWindowSample.h>

namespace Zeron
{
	namespace Render
	{
		class Mesh;
	}
	namespace Gfx
	{
		class Buffer;
		class PipelineBinding;
		class Pipeline;
	}
	namespace Physics
	{
		class Material;
		class Engine;
		class World;
		class DynamicBody;
		class CollisionBody;
	}

	class Window;
}

namespace Sandbox
{
	struct SampleContext;

	class WireframePhysicsSample : public IWindowSample {
	  public:
		static constexpr char NAME[] = "Wireframe Physics";

		WireframePhysicsSample(IWindowSampleContext& ctx);
		~WireframePhysicsSample();

		void Init() override;
		void Tick() override;
		void OnGui() override;
		void Draw(Zeron::Gfx::CommandBuffer& cmd) override;

	  private:
		std::shared_ptr<Zeron::Gfx::ShaderProgram> mShader;
		std::unique_ptr<Zeron::Gfx::Pipeline> mPipeline;
		std::unique_ptr<Zeron::Gfx::PipelineBinding> mPipelineBinding;
		std::unique_ptr<Zeron::Render::Mesh> mDebugMesh;


		std::unique_ptr<Zeron::Physics::Engine> mPhysics;
		std::unique_ptr<Zeron::Physics::World> mPhysicsWorld;
		std::unique_ptr<Zeron::Physics::DynamicBody> mDynamicBody;
		std::unique_ptr<Zeron::Physics::Material> mDynamicMaterial;
		std::unique_ptr<Zeron::Physics::CollisionBody> mStaticBody;
	};
}