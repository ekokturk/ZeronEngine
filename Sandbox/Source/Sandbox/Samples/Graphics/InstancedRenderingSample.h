// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Entity/EntityManager.h>
#include <Render/Light/Light.h>
#include <Sandbox/Samples/IWindowSample.h>


namespace Zeron
{
	namespace Render
	{
		class Mesh;
		struct Light;
		class Model;
	}
	namespace Gfx
	{
		class FrameBuffer;
		class RenderPass;
		class Pipeline;
		class ShaderProgram;
		class Texture;
		class Sampler;
		class PipelineBinding;
		class Buffer;
	}
	namespace Asset
	{
		class Image;
	}
}

namespace Sandbox
{
	struct SampleContext;

	class InstancedRenderingSample : public IWindowSample {
	  public:
		static constexpr char NAME[] = "Instanced Rendering";

		InstancedRenderingSample(IWindowSampleContext& ctx);
		~InstancedRenderingSample();
		void Init() override;
		void Tick() override;
		void OnFrameBegin(Zeron::Gfx::CommandBuffer& cmd) override;
		void OnFrameEnd(Zeron::Gfx::CommandBuffer& cmd) override;
		void Draw(Zeron::Gfx::CommandBuffer& cmd) override;
		void OnGui() override;
		void OnWindowResized(const Zeron::Vec2i& size) override;

	  private:
		struct MeshComponent {
			std::vector<const Zeron::Render::Mesh*> mMeshes;
			std::vector<Zeron::Mat4> mTransforms;
		};

		struct TransformComponent {
			Zeron::Mat4 mTransform;
		};

		struct alignas(16) MeshInstanceData {
			Zeron::Mat4 mLocalTransform;
			Zeron::Mat4 mParentTransform;
		};

		struct PerCameraData {
			Zeron::Mat4 mViewProjection;
			Zeron::Mat4 mView;
			Zeron::Mat4 mProjection;
		};

		int mInstanceM = 10;
		int mInstanceN = 10;

		std::unique_ptr<Zeron::Gfx::Buffer> mPerCameraBuffer;
		std::unique_ptr<Zeron::Gfx::Buffer> mMeshInstanceDataBuffer;

		void _createFrameResources();
		void _doShadowPass(Zeron::Gfx::CommandBuffer& cmd);

		// Model
		std::unique_ptr<Zeron::Render::Model> mModel;
		std::unique_ptr<Zeron::Gfx::Sampler> mSampler;
		std::shared_ptr<Zeron::Gfx::ShaderProgram> mShader;
		std::unique_ptr<Zeron::Gfx::Texture> mDiffuseTexture;
		std::unique_ptr<Zeron::Gfx::Texture> mNormalTexture;
		std::unique_ptr<Zeron::Gfx::Pipeline> mPipeline;
		std::unique_ptr<Zeron::Gfx::PipelineBinding> mMeshPipelineBinding;
		std::unique_ptr<Zeron::Asset::Image> mDiffuseImage;
		std::unique_ptr<Zeron::Asset::Image> mNormalImage;

		// Lights
		struct alignas(16) LightingInfo {
			Zeron::Mat4 mShadowMVP;
			int mHasShadows = 1;
			Zeron::Vec3 mAmbientLightColor = Zeron::Vec3::ONE;
			float mAmbientLightStrength = 0.1f;
			int mLightCount = 0;
		} mLightingInfo;

		std::unique_ptr<Zeron::Gfx::Buffer> mLightListBuffer;
		std::unique_ptr<Zeron::Gfx::Buffer> mLightingInfoBuffer;
		std::vector<Zeron::Render::Light> mLightList;

		// Shadow

		std::shared_ptr<Zeron::Gfx::ShaderProgram> mShadowShader;
		std::shared_ptr<Zeron::Gfx::Texture> mShadowTexture;
		std::shared_ptr<Zeron::Gfx::Buffer> mShadowMVPBuffer;
		std::shared_ptr<Zeron::Gfx::RenderPass> mShadowRenderPass;
		std::unique_ptr<Zeron::Gfx::Pipeline> mShadowPipeline;
		std::unique_ptr<Zeron::Gfx::PipelineBinding> mShadowPipelineBinding;
		std::unique_ptr<Zeron::Gfx::FrameBuffer> mShadowFrameBuffer;

		std::unique_ptr<Zeron::Render::Mesh> mPlane;

		Zeron::EntityManager mEntityManager;
		std::map<const Zeron::Render::Mesh*, std::vector<MeshInstanceData>> mMeshRenderBatch;
	};
}